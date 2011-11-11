if (file.exists("../src/yaml.so")) {
  dyn.load("../src/yaml.so")
  for (f in list.files("../R", full.names = TRUE)) {
    source(f)
  }
} else {
  library(yaml)
}

assert <- function(bool) {
  if (!bool) stop(bool, " is not TRUE")
}
assert_equal <- function(expected, actual) {
  if (any(!identical(expected, actual))) {
    estr <- paste(capture.output(str(expected)), collapse="\n    ");
    astr <- paste(capture.output(str(actual)), collapse="\n    ");
    stop(paste("", "Expected:", estr, "got:", astr, sep="\n  "))
  }
}
assert_nan <- function(value) {
  if (!is.nan(value)) stop(value, " is not NaN")
}
assert_lists_equal <- function(expected, actual) {
  if (any(sort(names(expected)) != sort(names(actual)))) stop("Lists are not equal (names differ)")
  if (any(class(expected) != class(actual))) stop("Lists are not equal (classes differ)")

  for (n in names(expected)) {
    e <- expected[[n]]
    a <- actual[[n]]
    if (class(e) == "list" && class(a) == "list") assert_lists_equal(e, a)
    else if (e != a) stop("Lists are not equal (elements differ)")
  }
}
