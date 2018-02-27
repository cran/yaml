#include "r_ext.h"

extern SEXP R_KeysSymbol;
extern SEXP R_IdenticalFunc;
extern SEXP R_Sentinel;
extern SEXP R_SequenceStart;
extern SEXP R_MappingStart;
extern SEXP R_MappingEnd;
extern char error_msg[ERROR_MSG_SIZE];

/* Compare two R objects (with the R identical function).
 * Returns 0 or 1 */
static int
R_cmp(s_first, s_second)
  SEXP s_first;
  SEXP s_second;
{
  int i = 0, retval = 0, *arr = NULL;
  SEXP s_call = NULL, s_result = NULL, s_bool = NULL;

  PROTECT(s_bool = allocVector(LGLSXP, 1));
  LOGICAL(s_bool)[0] = 1;
  PROTECT(s_call = LCONS(R_IdenticalFunc, list4(s_first, s_second, s_bool, s_bool)));
  PROTECT(s_result = eval(s_call, R_GlobalEnv));

  arr = LOGICAL(s_result);
  for(i = 0; i < length(s_result); i++) {
    if (!arr[i]) {
      retval = 1;
      break;
    }
  }
  UNPROTECT(3);
  return retval;
}

/* Returns the index of the first instance of needle in haystack */
static int
R_index(s_haystack, s_needle, character, upper_bound)
  SEXP s_haystack;
  SEXP s_needle;
  int character;
  int upper_bound;
{
  int i = 0;

  if (character) {
    for (i = 0; i < upper_bound; i++) {
      if (strcmp(CHAR(s_needle), CHAR(STRING_ELT(s_haystack, i))) == 0) {
        return i;
      }
    }
  }
  else {
    for (i = 0; i < upper_bound; i++) {
      if (R_cmp(s_needle, VECTOR_ELT(s_haystack, i)) == 0) {
        return i;
      }
    }
  }

  return -1;
}

/* Returns true if obj is a list with a keys attribute */
static int
R_is_pseudo_hash(s_obj)
  SEXP s_obj;
{
  SEXP s_keys = NULL;
  if (TYPEOF(s_obj) != VECSXP)
    return 0;

  s_keys = getAttrib(s_obj, R_KeysSymbol);
  return (s_keys != R_NilValue && TYPEOF(s_keys) == VECSXP);
}

/* Set a character attribute on an R object */
static void
R_set_str_attrib(s_obj, s_sym, str)
  SEXP s_obj;
  SEXP s_sym;
  char *str;
{
  SEXP s_val = NULL;
  PROTECT(s_val = ScalarString(mkCharCE(str, CE_UTF8)));
  setAttrib(s_obj, s_sym, s_val);
  UNPROTECT(1);
}

/* Set the R object's class attribute */
static void
R_set_class(s_obj, name)
  SEXP s_obj;
  char *name;
{
  R_set_str_attrib(s_obj, R_ClassSymbol, name);
}

/* Get the type part of the tag, throw away any !'s */
static char *
process_tag(tag)
  char *tag;
{
  char *retval = tag;

  if (strncmp(retval, "tag:yaml.org,2002:", 18) == 0) {
    retval = retval + 18;
  }
  else {
    while (*retval == '!') {
      retval++;
    }
  }
  return retval;
}

static SEXP
find_handler(s_handlers, name)
  SEXP s_handlers;
  const char *name;
{
  SEXP s_names = NULL, s_retval = R_NilValue;
  int i = 0;

  /* Look for a custom R handler */
  if (s_handlers != R_NilValue) {
    PROTECT(s_names = GET_NAMES(s_handlers));
    for (i = 0; i < length(s_names); i++) {
      if (STRING_ELT(s_names, i) != NA_STRING) {
        if (strcmp(translateChar(STRING_ELT(s_names, i)), name) == 0) {
          /* Found custom handler */
          s_retval = VECTOR_ELT(s_handlers, i);
          break;
        }
      }
    }
    UNPROTECT(1); /* s_names */
  }

  return s_retval;
}

static int
run_handler(s_handler, s_arg, s_result)
  SEXP s_handler;
  SEXP s_arg;
  SEXP *s_result;
{
  SEXP s_cmd = NULL;
  int err = 0;

  PROTECT(s_cmd = lang2(s_handler, s_arg));
  *s_result = R_tryEval(s_cmd, R_GlobalEnv, &err);
  UNPROTECT(1);

  return err;
}

static int
handle_alias(event, s_stack, s_aliases)
  yaml_event_t *event;
  SEXP *s_stack;
  SEXP s_aliases;
{
  SEXP s_curr = NULL, s_obj = NULL;
  int handled = 0;
  const char *name = NULL, *anchor = NULL;

  /* Try to find object with the supplied anchor */
  s_curr = s_aliases;
  s_obj = CAR(s_curr);
  anchor = (const char *)event->data.alias.anchor;
  while (s_obj != R_Sentinel) {
    name = CHAR(TAG(s_curr));
    if (strcmp(name, anchor) == 0) {
      /* Found object, push onto stack */
      *s_stack = CONS(s_obj, *s_stack);
      MARK_NOT_MUTABLE(s_obj);
      handled = 1;
      break;
    }
    s_curr = CDR(s_curr);
    s_obj = CAR(s_curr);
  }

  if (!handled) {
    warning("Unknown anchor: %s", (char *)event->data.alias.anchor);
    PROTECT(s_obj = ScalarString(mkChar("_yaml.bad-anchor_")));
    R_set_class(s_obj, "_yaml.bad-anchor_");
    UNPROTECT(1);
    *s_stack = CONS(s_obj, *s_stack);
  }

  return 0;
}

static int
handle_scalar(event, s_stack, s_handlers)
  yaml_event_t *event;
  SEXP *s_stack;
  SEXP s_handlers;
{
  SEXP s_obj = NULL, s_handler = NULL, s_new_obj = NULL, s_expr = NULL;
  const char *value = NULL, *tag = NULL, *nptr = NULL;
  char *endptr = NULL;
  size_t len = 0;
  int handled = 0, coercion_err = 0, base = 0, n = 0;
  long int long_n = 0;
  double f = 0.0f;
  ParseStatus parse_status;

  tag = (const char *)event->data.scalar.tag;
  value = (const char *)event->data.scalar.value;
  if (tag == NULL || strcmp(tag, "!") == 0) {
    /* There's no tag! */

    /* If this is a quoted string, leave it as a string */
    switch (event->data.scalar.style) {
      case YAML_SINGLE_QUOTED_SCALAR_STYLE:
      case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
        tag = "str";
        break;
      default:
        /* Try to tag it */
        len = event->data.scalar.length;
        tag = find_implicit_tag(value, len);
    }
  }
  else {
    tag = process_tag(tag);
  }

#if DEBUG
  Rprintf("Value: (%s), Tag: (%s)\n", value, tag);
#endif

  /* 'Vanilla' object */
  PROTECT(s_obj = ScalarString(mkCharCE(value, CE_UTF8)));

  /* Look for a custom R handler */
  s_handler = find_handler(s_handlers, (const char *)tag);
  if (s_handler != R_NilValue) {
    if (run_handler(s_handler, s_obj, &s_new_obj) != 0) {
      warning("an error occurred when handling type '%s'; using default handler", tag);
    }
    else {
      handled = 1;
    }
  }

  if (!handled) {
    /* Default handlers */

    if (strcmp(tag, "str") == 0) {
      /* already a string */
    }
    else if (strcmp(tag, "seq") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "int#na") == 0) {
      s_new_obj = ScalarInteger(NA_INTEGER);
    }
    else if (strcmp(tag, "int") == 0 || strncmp(tag, "int#", 4) == 0) {
      base = -1;
      if (strcmp(tag, "int") == 0) {
        base = 10;
      }
      else if (strcmp(tag, "int#hex") == 0) {
        base = 16;
      }
      else if (strcmp(tag, "int#oct") == 0) {
        base = 8;
      }

      if (base >= 0) {
        errno = 0;
        nptr = CHAR(STRING_ELT(s_obj, 0));
        long_n = strtol(nptr, &endptr, base);
        if (*endptr != 0) {
          /* strtol is perfectly happy converting partial strings to
           * integers, but R isn't. If you call as.integer() on a
           * string that isn't completely an integer, you get back
           * an NA. So I'm reproducing that behavior here. */

          warning("NAs introduced by coercion: %s is not an integer", nptr);
          n = NA_INTEGER;
        } else if (errno == ERANGE) {
          warning("NAs introduced by coercion: %s is out of integer range", nptr);
          n = NA_INTEGER;
        } else if (long_n < INT_MIN || long_n > INT_MAX || (int)long_n == NA_INTEGER) {
          warning("NAs introduced by coercion: %s is out of integer range", nptr);
          n = NA_INTEGER;
        } else {
          n = (int)long_n;
        }

        s_new_obj = ScalarInteger(n);
      }
      else {
        /* unknown integer base */
        coercion_err = 1;
      }
    }
    else if (strcmp(tag, "float") == 0 || strcmp(tag, "float#fix") == 0 || strcmp(tag, "float#exp") == 0) {
      errno = 0;
      nptr = CHAR(STRING_ELT(s_obj, 0));
      f = strtod(nptr, &endptr);
      if (*endptr != 0) {
        /* No valid floats found (see note above about integers) */
        warning("NAs introduced by coercion: %s is not a real", nptr);
        f = NA_REAL;
      } else if (errno == ERANGE || f == NA_REAL) {
        warning("NAs introduced by coercion: %s is out of real range", nptr);
        f = NA_REAL;
      }

      s_new_obj = ScalarReal(f);
    }
    else if (strcmp(tag, "bool#yes") == 0) {
      s_new_obj = ScalarLogical(TRUE);
    }
    else if (strcmp(tag, "bool#no") == 0) {
      s_new_obj = ScalarLogical(FALSE);
    }
    else if (strcmp(tag, "bool#na") == 0) {
      s_new_obj = ScalarLogical(NA_LOGICAL);
    }
    else if (strcmp(tag, "omap") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "merge") == 0) {
      /* see http://yaml.org/type/merge.html */
      PROTECT(s_new_obj = ScalarString(mkChar("_yaml.merge_")));
      R_set_class(s_new_obj, "_yaml.merge_");
      UNPROTECT(1);
    }
    else if (strcmp(tag, "float#na") == 0) {
      s_new_obj = ScalarReal(NA_REAL);
    }
    else if (strcmp(tag, "float#nan") == 0) {
      s_new_obj = ScalarReal(R_NaN);
    }
    else if (strcmp(tag, "float#inf") == 0) {
      s_new_obj = ScalarReal(R_PosInf);
    }
    else if (strcmp(tag, "float#neginf") == 0) {
      s_new_obj = ScalarReal(R_NegInf);
    }
    else if (strcmp(tag, "str#na") == 0) {
      s_new_obj = ScalarString(NA_STRING);
    }
    else if (strcmp(tag, "null") == 0) {
      s_new_obj = R_NilValue;
    }
    else if (strcmp(tag, "expr") == 0) {
      PROTECT(s_obj);
      s_expr = R_ParseVector(s_obj, 1, &parse_status, R_NilValue);
      UNPROTECT(1);

      if (parse_status != PARSE_OK) {
        coercion_err = 1;
        set_error_msg("Could not parse expression: %s", CHAR(STRING_ELT(s_obj, 0)));
      }
      else {
        /* NOTE: R_tryEval will not return if R_Interactive is FALSE. */
        PROTECT(s_expr);
        s_new_obj = R_tryEval(VECTOR_ELT(s_expr, 0), R_GlobalEnv, &coercion_err);
        UNPROTECT(1);

        if (coercion_err) {
          set_error_msg("Could not evaluate expression: %s", CHAR(STRING_ELT(s_obj, 0)));
        } else {
          warning("R expressions in yaml.load will not be auto-evaluated by default in the near future");
        }
      }
    }
  }
  UNPROTECT(1); /* s_obj */

  if (coercion_err == 1) {
    if (error_msg[0] == 0) {
      set_error_msg("Invalid tag for scalar: %s", tag);
    }
    return 1;
  }

  *s_stack = CONS(s_new_obj == NULL ? s_obj : s_new_obj, *s_stack);

  return 0;
}

static void
handle_structure_start(event, s_stack, is_map)
  yaml_event_t *event;
  SEXP *s_stack;
  int is_map;
{
  SEXP s_sym = NULL, s_tag_obj = NULL, s_anchor_obj = NULL, s_tag = NULL;
  yaml_char_t *tag = NULL, *anchor = NULL;

  if (is_map) {
    s_sym = R_MappingStart;
    tag = event->data.mapping_start.tag;
    anchor = event->data.mapping_start.anchor;
  } else {
    s_sym = R_SequenceStart;
    tag = event->data.sequence_start.tag;
    anchor = event->data.sequence_start.anchor;
  }

  *s_stack = CONS(s_sym, *s_stack);

  /* Create pairlist tag */
  if (tag == NULL) {
    s_tag_obj = R_NilValue;
  }
  else {
    s_tag_obj = mkChar((const char *) tag);
  }
  if (anchor == NULL) {
    s_anchor_obj = R_NilValue;
  }
  else {
    PROTECT(s_tag_obj);
    s_anchor_obj = mkChar((const char *) anchor);
    UNPROTECT(1);
  }
  s_tag = list2(s_tag_obj, s_anchor_obj);
  SET_TAG(*s_stack, s_tag);
}

static int
handle_sequence(event, s_stack, s_handlers, coerce_keys)
  yaml_event_t *event;
  SEXP *s_stack;
  SEXP s_handlers;
  int coerce_keys;
{
  SEXP s_curr = NULL, s_obj = NULL, s_list = NULL, s_handler = NULL,
       s_new_obj = NULL, s_keys = NULL, s_key = NULL, s_tag = NULL;
  int count = 0, i = 0, j = 0, type = 0, child_type = 0, handled = 0,
      coercion_err = 0, len = 0, total_len = 0, dup_key = 0, idx = 0,
      obj_len = 0;
  const char *tag = NULL;

  /* Find out how many elements there are */
  s_curr = *s_stack;
  count = 0;
  while (CAR(s_curr) != R_SequenceStart) {
    count++;
    s_curr = CDR(s_curr);
  }
  s_tag = CAR(TAG(s_curr));
  tag = s_tag == R_NilValue ? NULL : CHAR(s_tag);

  /* Initialize list */
  PROTECT(s_list = allocVector(VECSXP, count));

  /* Populate the list, popping items off the stack as we go */
  type = -2;
  s_curr = *s_stack;
  for (i = count - 1; i >= 0; i--) {
    s_obj = CAR(s_curr);
    s_curr = CDR(s_curr);
    SET_VECTOR_ELT(s_list, i, s_obj);

    /* Treat primitive vectors with more than one element as a list for
     * coercion purposes. */
    child_type = TYPEOF(s_obj);
    switch (child_type) {
      case LGLSXP:
      case INTSXP:
      case REALSXP:
      case STRSXP:
        if (length(s_obj) != 1) {
          child_type = VECSXP;
        }
        break;
    }

    if (type == -2) {
      type = child_type;
    }
    else if (type != -1 && child_type != type) {
      type = -1;
    }
  }

  /* Tags! */
  if (tag == NULL) {
    tag = "seq";
  }
  else {
    tag = process_tag(tag);
  }

  /* Look for a custom R handler */
  s_handler = find_handler(s_handlers, (const char *)tag);
  if (s_handler != R_NilValue) {
    if (run_handler(s_handler, s_list, &s_new_obj) != 0) {
      warning("an error occurred when handling type '%s'; using default handler", tag);
    }
    else {
      handled = 1;
    }
  }

  if (!handled) {
    /* default handlers, ordered by most-used */

    if (strcmp(tag, "seq") == 0) {
      /* Let's try to coerce this list! */
      switch (type) {
        case LGLSXP:
        case INTSXP:
        case REALSXP:
        case STRSXP:
          s_new_obj = coerceVector(s_list, type);
          break;
      }
    }
    else if (strcmp(tag, "str") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "int#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "int") == 0 || strncmp(tag, "int#", 4) == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float") == 0 || strcmp(tag, "float#fix") == 0 || strcmp(tag, "float#exp") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "bool#yes") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "bool#no") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "bool#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "omap") == 0) {
      /* NOTE: This is here mostly because of backwards compatibility
       * with R yaml 1.x package. All maps are ordered in 2.x, so there's
       * no real need to use omap */

      len = length(s_list);
      total_len = 0;
      for (i = 0; i < len; i++) {
        s_obj = VECTOR_ELT(s_list, i);
        if ((coerce_keys && !R_is_named_list(s_obj)) || (!coerce_keys && !R_is_pseudo_hash(s_obj))) {
          set_error_msg("omap must be a sequence of maps");
          coercion_err = 1;
          break;
        }
        total_len += length(s_obj);
      }

      /* Construct the list! */
      if (!coercion_err) {
        PROTECT(s_new_obj = allocVector(VECSXP, total_len));
        if (coerce_keys) {
          s_keys = allocVector(STRSXP, total_len);
          SET_NAMES(s_new_obj, s_keys);
        }
        else {
          s_keys = allocVector(VECSXP, total_len);
          setAttrib(s_new_obj, R_KeysSymbol, s_keys);
        }

        for (i = 0, idx = 0; i < len && dup_key == 0; i++) {
          s_obj = VECTOR_ELT(s_list, i);
          obj_len = length(s_obj);
          for (j = 0; j < obj_len && dup_key == 0; j++) {
            SET_VECTOR_ELT(s_new_obj, idx, VECTOR_ELT(s_obj, j));

            if (coerce_keys) {
              PROTECT(s_key = STRING_ELT(GET_NAMES(s_obj), j));
              SET_STRING_ELT(s_keys, idx, s_key);

              if (R_index(s_keys, s_key, 1, idx) >= 0) {
                dup_key = 1;
                set_error_msg("Duplicate omap key: '%s'", CHAR(s_key));
              }
              UNPROTECT(1); /* s_key */
            }
            else {
              s_key = VECTOR_ELT(getAttrib(s_obj, R_KeysSymbol), j);
              SET_VECTOR_ELT(s_keys, idx, s_key);

              if (R_index(s_keys, s_key, 0, idx) >= 0) {
                dup_key = 1;
                set_error_msg("Duplicate omap key: %s", R_inspect(s_key));
              }
            }
            idx++;
          }
        }
        UNPROTECT(1); /* s_new_obj */

        if (dup_key == 1) {
          coercion_err = 1;
        }
      }
    }
    else if (strcmp(tag, "merge") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#nan") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#inf") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#neginf") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "str#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "null") == 0) {
      s_new_obj = R_NilValue;
    }
    else if (strcmp(tag, "expr") == 0) {
      coercion_err = 1;
    }
  }
  UNPROTECT(1); /* s_list */

  if (coercion_err == 1) {
    if (error_msg[0] == 0) {
      set_error_msg("Invalid tag: %s for sequence", tag);
    }
    return 1;
  }

  /* s_curr is at sequence start */
  *s_stack = s_curr;
  SETCAR(*s_stack, s_new_obj == NULL ? s_list : s_new_obj);

  return 0;
}

static SEXP
find_map_entry(s_map, s_key, character)
  SEXP s_map;
  SEXP s_key;
  int character;
{
  SEXP s_curr = NULL, s_prev = NULL;

  s_curr = s_map;
  if (character) {
    while (CAR(s_curr) != R_MappingEnd) {
      if (strcmp(CHAR(s_key), CHAR(CAR(TAG(s_curr)))) == 0) {
        return list2(s_curr, s_prev == NULL ? R_NilValue : s_prev);
      }
      s_prev = s_curr;
      s_curr = CDR(s_curr);
    }
  }
  else {
    while (CAR(s_curr) != R_MappingEnd) {
      if (R_cmp(s_key, CAR(TAG(s_curr))) == 0) {
        return list2(s_curr, s_prev == NULL ? R_NilValue : s_prev);
      }
      s_prev = s_curr;
      s_curr = CDR(s_curr);
    }
  }

  return NULL;
}

static int
expand_merge(s_merge_list, s_map, coerce_keys)
  SEXP s_merge_list;
  SEXP *s_map;
  int coerce_keys;
{
  SEXP s_merge_keys = NULL, s_value = NULL, s_key = NULL, s_entry = NULL,
       s_entry_parent = NULL, s_result = NULL;
  int i = 0, count = 0;

  s_merge_keys = coerce_keys ? GET_NAMES(s_merge_list) : getAttrib(s_merge_list, R_KeysSymbol);
  for (i = length(s_merge_list) - 1; i >= 0; i--) {
    s_value = VECTOR_ELT(s_merge_list, i);
    if (coerce_keys) {
      s_key = STRING_ELT(s_merge_keys, i);
    }
    else {
      s_key = VECTOR_ELT(s_merge_keys, i);
    }

    PROTECT(s_key);
    s_result = find_map_entry(*s_map, s_key, coerce_keys);
    if (s_result != NULL) {
      s_entry = CAR(s_result);
      s_entry_parent = CADR(s_result);

      /* A matching key is already in the map. If the existing key is from a
       * merge, it's okay to override it. If not, it's a duplicate key error. */
      if (LOGICAL(CADR(TAG(s_entry)))[0] == FALSE) {
        set_error_msg("Duplicate map key: '%s'", coerce_keys ? CHAR(s_key) : R_inspect(s_key));
        UNPROTECT(1); /* s_key */
        return -1;
      } else {
        warning("Duplicate map key ignored during merge: '%s'",
            coerce_keys ? CHAR(s_key) : R_inspect(s_key));

        /* Unlink earlier entry. */
        if (s_entry_parent == R_NilValue) {
          *s_map = CDR(s_entry);
        } else {
          SETCDR(s_entry_parent, CDR(s_entry));
        }
        count--;
      }
    }

    *s_map = CONS(s_value, *s_map);
    SET_TAG(*s_map, list2(s_key, ScalarLogical(TRUE)));
    count++;
    UNPROTECT(1); /* s_key */
  }

  return count;
}

static int
is_mergeable(s_merge_list, coerce_keys)
  SEXP s_merge_list;
  int coerce_keys;
{
  return (coerce_keys && R_is_named_list(s_merge_list)) ||
    (!coerce_keys && R_is_pseudo_hash(s_merge_list));
}

static int
handle_map(event, s_stack, s_handlers, coerce_keys)
  yaml_event_t *event;
  SEXP *s_stack;
  SEXP s_handlers;
  int coerce_keys;
{
  SEXP s_list = NULL, s_keys = NULL, s_key = NULL, s_value = NULL,
       s_obj = NULL, s_map = NULL, s_new_obj = NULL, s_handler = NULL,
       s_tag = NULL, s_entry = NULL, s_entry_parent = NULL, s_result = NULL;
  int count = 0, i = 0, map_err = 0, handled = 0, coercion_err = 0, len = 0;
  const char *tag = NULL;

  /* Iterate keys and values (backwards) */
  PROTECT(s_map = list1(R_MappingEnd));
  while (!map_err && CAR(*s_stack) != R_MappingStart) {
    s_value = CAR(*s_stack);
    s_key = CADR(*s_stack);
    *s_stack = CDDR(*s_stack);

    if (R_has_class(s_key, "_yaml.merge_")) {
      if (is_mergeable(s_value, coerce_keys)) {
        /* i.e.
         *    - &bar { hey: dude }
         *    - foo:
         *        hello: friend
         *        <<: *bar
         */
        len = expand_merge(s_value, &s_map, coerce_keys);
        if (len >= 0) {
          count += len;
        } else {
          map_err = 1;
        }
      }
      else if (TYPEOF(s_value) == VECSXP) {
        /* i.e.
         *    - &bar { hey: dude }
         *    - &baz { hi: buddy }
         *    - foo:
         *        hello: friend
         *        <<: [*bar, *baz]
         */

        /* Go backwards for consistency */
        for (i = length(s_value) - 1; i >= 0; i--) {
          s_obj = VECTOR_ELT(s_value, i);
          if (is_mergeable(s_obj, coerce_keys)) {
            len = expand_merge(s_obj, &s_map, coerce_keys);
            if (len >= 0) {
              count += len;
            } else {
              map_err = 1;
              break;
            }
          }
          else {
            /* Illegal merge */
            set_error_msg("Illegal merge: %s", R_inspect(s_value));
            map_err = 1;
            break;
          }
        }
      }
      else {
        /* Illegal merge */
        set_error_msg("Illegal merge: %s", R_inspect(s_value));
        map_err = 1;
      }
    }
    else {
      /* Normal map entry */
      if (coerce_keys) {
        /* (Possibly) convert this key to a character vector, and then save
         * the first element in the vector (CHARSXP element). Throw away
         * the containing vector, since we don't need it anymore. */
        PROTECT(s_key = AS_CHARACTER(s_key));
        len = length(s_key);

        if (len == 0) {
          warning("Empty character vector used as a list name");
          s_key = mkChar("");
        } else {
          if (len > 1) {
            warning("Character vector of length greater than 1 used as a list name");
          }
          s_key = STRING_ELT(s_key, 0);
        }
        UNPROTECT(1);
      }

      PROTECT(s_key);
      s_result = find_map_entry(s_map, s_key, coerce_keys);
      if (s_result != NULL) {
        s_entry = CAR(s_result);
        s_entry_parent = CADR(s_result);

        /* A matching key is already in the map. If the existing key is from a
         * merge, it's okay to override it. If not, it's a duplicate key error. */
        s_tag = TAG(s_entry);
        if (LOGICAL(CADR(s_tag))[0] == FALSE) {
          set_error_msg("Duplicate map key: '%s'", coerce_keys ? CHAR(s_key) : R_inspect(s_key));
          map_err = 1;
        } else {
          /* Unlink earlier entry. */
          if (s_entry_parent == R_NilValue) {
            s_map = CDR(s_entry);
          } else {
            SETCDR(s_entry_parent, CDR(s_entry));
          }
          count--;
        }
      }

      if (!map_err) {
        s_map = CONS(s_value, s_map);
        SET_TAG(s_map, list2(s_key, ScalarLogical(FALSE)));
        count++;
      }
      UNPROTECT(1); /* s_key */
    }
  }

  if (map_err) {
    UNPROTECT(1); /* s_map */
    return 1;
  }

  /* Initialize value list */
  PROTECT(s_list = allocVector(VECSXP, count));

  /* Initialize key list/vector */
  if (coerce_keys) {
    s_keys = NEW_STRING(count);
    SET_NAMES(s_list, s_keys);
  }
  else {
    s_keys = allocVector(VECSXP, count);
    setAttrib(s_list, R_KeysSymbol, s_keys);
  }

  /* Iterate map entries (forward) */
  s_entry = s_map;
  for (i = 0; i < count; i++) {
    s_value = CAR(s_entry);
    s_key = CAR(TAG(s_entry));
    s_entry = CDR(s_entry);

    SET_VECTOR_ELT(s_list, i, s_value);

    /* map key */
    if (coerce_keys) {
      SET_STRING_ELT(s_keys, i, s_key);
    }
    else {
      SET_VECTOR_ELT(s_keys, i, s_key);
    }
  }
  UNPROTECT(2); /* s_map, s_list */

  /* Tags! */
  s_tag = CAR(TAG(*s_stack));
  tag = s_tag == R_NilValue ? NULL : CHAR(s_tag);
  if (tag == NULL) {
    tag = "map";
  }
  else {
    tag = process_tag(tag);
  }

  /* Look for a custom R handler */
  PROTECT(s_list);
  s_handler = find_handler(s_handlers, (const char *) tag);
  if (s_handler != R_NilValue) {
    if (run_handler(s_handler, s_list, &s_new_obj) != 0) {
      warning("an error occurred when handling type '%s'; using default handler", tag);
    }
    else {
      handled = 1;
    }
  }
  UNPROTECT(1); /* s_list */

  if (!handled) {
    /* default handlers, ordered by most-used */

    if (strcmp(tag, "map") == 0) {
      /* already a map */
    }
    else if (strcmp(tag, "str") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "seq") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "int#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "int") == 0 || strncmp(tag, "int#", 4) == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float") == 0 || strcmp(tag, "float#fix") == 0 || strcmp(tag, "float#exp") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "bool#yes") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "bool#no") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "bool#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "omap") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "merge") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#nan") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#inf") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "float#neginf") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "str#na") == 0) {
      coercion_err = 1;
    }
    else if (strcmp(tag, "null") == 0) {
      s_new_obj = R_NilValue;
    }
    else if (strcmp(tag, "expr") == 0) {
      coercion_err = 1;
    }
  }

  if (coercion_err == 1) {
    if (error_msg[0] == 0) {
      set_error_msg("Invalid tag: %s for map");
    }
    return 1;
  }

  SETCAR(*s_stack, s_new_obj == NULL ? s_list : s_new_obj);

  return 0;
}

static void
possibly_record_alias(s_anchor, s_aliases, s_obj)
  SEXP s_anchor;
  SEXP *s_aliases;
  SEXP s_obj;
{
  if (s_anchor == NULL || TYPEOF(s_anchor) != CHARSXP) return;

  *s_aliases = CONS(s_obj, *s_aliases);
  SET_TAG(*s_aliases, s_anchor);
}

SEXP
R_unserialize_from_yaml(s_str, s_use_named, s_handlers, s_error_label)
  SEXP s_str;
  SEXP s_use_named;
  SEXP s_handlers;
  SEXP s_error_label;
{
  SEXP s_retval = NULL, s_handler = NULL, s_names = NULL, s_handlers_2 = NULL,
       s_stack = NULL, s_aliases = NULL, s_anchor = NULL;
  yaml_parser_t parser;
  yaml_event_t event;
  const char *str = NULL, *name = NULL, *error_label = NULL;
  char *error_msg_copy = NULL;
  long len = 0;
  int use_named = 0, i = 0, done = 0, err = 0;

  if (!isString(s_str) || length(s_str) != 1) {
    error("first argument must be a character vector of length 1");
    return R_NilValue;
  }

  if (!isLogical(s_use_named) || length(s_use_named) != 1) {
    error("second argument must be a logical vector of length 1");
    return R_NilValue;
  }

  if (s_handlers == R_NilValue) {
    // Do nothing
  }
  else if (!R_is_named_list(s_handlers)) {
    error("handlers must be either NULL or a named list of functions");
    return R_NilValue;
  }
  else {
    PROTECT(s_handlers_2 = allocVector(VECSXP, length(s_handlers)));
    s_names = GET_NAMES(s_handlers);
    SET_NAMES(s_handlers_2, s_names);
    for (i = 0; i < length(s_handlers); i++) {
      name = CHAR(STRING_ELT(s_names, i));
      s_handler = VECTOR_ELT(s_handlers, i);

      if (TYPEOF(s_handler) != CLOSXP) {
        warning("Your handler for type '%s' is not a function; using default", name);
        s_handler = R_NilValue;
      }
      else if (strcmp(name, "merge") == 0 || strcmp(name, "default") == 0) {
        /* custom handlers for merge and default are illegal */
        warning("Custom handling for type '%s' is not allowed; handler ignored", name);
        s_handler = R_NilValue;
      }

      SET_VECTOR_ELT(s_handlers_2, i, s_handler);
    }
    s_handlers = s_handlers_2;
    UNPROTECT(1);
  }

  if (s_error_label == R_NilValue) {
    error_label = NULL;
  }
  else if (!isString(s_error_label) || length(s_error_label) != 1) {
    error("error_label must be either NULL or a character vector of length 1");
    return R_NilValue;
  } else {
    error_label = CHAR(STRING_ELT(s_error_label, 0));
  }

  str = CHAR(STRING_ELT(s_str, 0));
  len = length(STRING_ELT(s_str, 0));
  use_named = LOGICAL(s_use_named)[0];

  yaml_parser_initialize(&parser);
  yaml_parser_set_input_string(&parser, (const unsigned char *)str, len);

  PROTECT(s_stack = list1(R_Sentinel));
  PROTECT(s_aliases = list1(R_Sentinel));
  PROTECT(s_handlers);
  error_msg[0] = 0;
  while (!done) {
    if (yaml_parser_parse(&parser, &event)) {
      err = 0;

      switch (event.type) {
        case YAML_NO_EVENT:
        case YAML_STREAM_START_EVENT:
        case YAML_DOCUMENT_START_EVENT:
        case YAML_DOCUMENT_END_EVENT:
          break;

        case YAML_ALIAS_EVENT:
#if DEBUG
          Rprintf("ALIAS: %s\n", event.data.alias.anchor);
#endif
          handle_alias(&event, &s_stack, s_aliases);
          break;

        case YAML_SCALAR_EVENT:
#if DEBUG
          Rprintf("SCALAR: %s (%s) [%s]\n", event.data.scalar.value, event.data.scalar.tag, event.data.scalar.anchor);
#endif
          err = handle_scalar(&event, &s_stack, s_handlers);
          if (!err) {
            s_anchor = NULL;
            if (event.data.scalar.anchor != NULL) {
              s_anchor = mkChar((char *) event.data.scalar.anchor);
            }
            possibly_record_alias(s_anchor, &s_aliases, CAR(s_stack));
          }
          break;

        case YAML_SEQUENCE_START_EVENT:
#if DEBUG
          Rprintf("SEQUENCE START: (%s) [%s]\n", event.data.sequence_start.tag, event.data.sequence_start.anchor);
#endif
          handle_structure_start(&event, &s_stack, 0);
          break;

        case YAML_SEQUENCE_END_EVENT:
#if DEBUG
          Rprintf("SEQUENCE END\n");
#endif
          err = handle_sequence(&event, &s_stack, s_handlers, use_named);
          if (!err) {
            s_anchor = CADR(TAG(s_stack));
            possibly_record_alias(s_anchor, &s_aliases, CAR(s_stack));
            SET_TAG(s_stack, R_NilValue);
          }
          break;

        case YAML_MAPPING_START_EVENT:
#if DEBUG
          Rprintf("MAPPING START: (%s) [%s]\n", event.data.mapping_start.tag, event.data.mapping_start.anchor);
#endif
          handle_structure_start(&event, &s_stack, 1);
          break;

        case YAML_MAPPING_END_EVENT:
#if DEBUG
          Rprintf("MAPPING END\n");
#endif
          err = handle_map(&event, &s_stack, s_handlers, use_named);
          if (!err) {
            s_anchor = CADR(TAG(s_stack));
            possibly_record_alias(s_anchor, &s_aliases, CAR(s_stack));
            SET_TAG(s_stack, R_NilValue);
          }

          break;

        case YAML_STREAM_END_EVENT:
          if (CAR(s_stack) != R_Sentinel) {
            s_retval = CAR(s_stack);
          }
          else {
            s_retval = R_NilValue;
          }

          done = 1;
          break;
      }

      if (err) {
        s_retval = R_NilValue;
        done = 1;
      }
    }
    else {
      s_retval = R_NilValue;

      /* Parser error */
      switch (parser.error) {
        case YAML_MEMORY_ERROR:
          set_error_msg("Memory error: Not enough memory for parsing");
          break;

        case YAML_READER_ERROR:
          if (parser.problem_value != -1) {
            set_error_msg("Reader error: %s: #%X at %d", parser.problem,
              parser.problem_value, (int)parser.problem_offset);
          }
          else {
            set_error_msg("Reader error: %s at %d", parser.problem,
              (int)parser.problem_offset);
          }
          break;

        case YAML_SCANNER_ERROR:
          if (parser.context) {
            set_error_msg("Scanner error: %s at line %d, column %d "
              "%s at line %d, column %d\n", parser.context,
              (int)parser.context_mark.line+1,
              (int)parser.context_mark.column+1,
              parser.problem, (int)parser.problem_mark.line+1,
              (int)parser.problem_mark.column+1);
          }
          else {
            set_error_msg("Scanner error: %s at line %d, column %d",
              parser.problem, (int)parser.problem_mark.line+1,
              (int)parser.problem_mark.column+1);
          }
          break;

        case YAML_PARSER_ERROR:
          if (parser.context) {
            set_error_msg("Parser error: %s at line %d, column %d "
              "%s at line %d, column %d", parser.context,
              (int)parser.context_mark.line+1,
              (int)parser.context_mark.column+1,
              parser.problem, (int)parser.problem_mark.line+1,
              (int)parser.problem_mark.column+1);
          }
          else {
            set_error_msg("Parser error: %s at line %d, column %d",
              parser.problem, (int)parser.problem_mark.line+1,
              (int)parser.problem_mark.column+1);
          }
          break;

        default:
          /* Couldn't happen unless there is an undocumented/unhandled error
           * from LibYAML. */
          set_error_msg("Internal error");
          break;
      }
      done = 1;
    }

    yaml_event_delete(&event);
  }
  yaml_parser_delete(&parser);

  if (error_msg[0] != 0) {
    /* Prepend label to error message if specified */
    if (error_label != NULL) {
      error_msg_copy = (char *)malloc(sizeof(char) * ERROR_MSG_SIZE);
      if (error_msg_copy == NULL) {
        set_error_msg("Ran out of memory!");
      } else {
        memcpy(error_msg_copy, error_msg, ERROR_MSG_SIZE);
        set_error_msg("(%s) %s", error_label, error_msg_copy);
        free(error_msg_copy);
      }
    }
    error(error_msg);
  }

  UNPROTECT(3); /* s_stack, s_aliases, s_handlers */

  return s_retval;
}
