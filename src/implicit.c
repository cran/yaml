/* Generated by re2c 3.0 */
#line 1 "implicit.re"
#include <stddef.h> // size_t
#include "yaml.h"

char *Ryaml_find_implicit_tag(const char *str, size_t len)
{
  /* This bit was taken from implicit.re, which is in the Syck library.
   *
   * Copyright (C) 2003 why the lucky stiff */

  const char *cursor, *marker;
  cursor = str;


#line 16 "implicit.c"
{
	char yych;
	yych = *cursor;
	switch (yych) {
		case 0x00: goto yy1;
		case '+': goto yy4;
		case '-': goto yy5;
		case '.': goto yy6;
		case '0': goto yy7;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy8;
		case '<': goto yy9;
		case '=': goto yy10;
		case 'F': goto yy11;
		case 'N': goto yy12;
		case 'O': goto yy13;
		case 'T': goto yy14;
		case 'Y': goto yy15;
		case 'f': goto yy16;
		case 'n': goto yy17;
		case 'o': goto yy18;
		case 't': goto yy19;
		case 'y': goto yy20;
		case '~': goto yy21;
		default: goto yy2;
	}
yy1:
	++cursor;
#line 55 "implicit.re"
	{   return "null"; }
#line 54 "implicit.c"
yy2:
	++cursor;
yy3:
#line 101 "implicit.re"
	{   return "str"; }
#line 60 "implicit.c"
yy4:
	yych = *(marker = ++cursor);
	switch (yych) {
		case '.': goto yy22;
		case '0': goto yy24;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy25;
		default: goto yy3;
	}
yy5:
	yych = *(marker = ++cursor);
	switch (yych) {
		case '.': goto yy27;
		case '0': goto yy24;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy25;
		default: goto yy3;
	}
yy6:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 0x00: goto yy28;
		case ',': goto yy29;
		case '.': goto yy31;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy32;
		case 'E':
		case 'e': goto yy33;
		case 'I': goto yy34;
		case 'N': goto yy35;
		case 'i': goto yy36;
		case 'n': goto yy37;
		default: goto yy3;
	}
yy7:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 0x00: goto yy38;
		case ',': goto yy39;
		case '.': goto yy32;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7': goto yy41;
		case '8':
		case '9': goto yy42;
		case ':': goto yy43;
		case 'x': goto yy44;
		default: goto yy3;
	}
yy8:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 0x00: goto yy38;
		case ',': goto yy25;
		case '.': goto yy32;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy45;
		case ':': goto yy43;
		default: goto yy3;
	}
yy9:
	yych = *(marker = ++cursor);
	switch (yych) {
		case '<': goto yy46;
		default: goto yy3;
	}
yy10:
	yych = *++cursor;
	if (yych <= 0x00) goto yy47;
	goto yy3;
yy11:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 'A': goto yy48;
		case 'a': goto yy49;
		default: goto yy3;
	}
yy12:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 0x00: goto yy50;
		case 'O':
		case 'o': goto yy51;
		case 'U': goto yy52;
		case 'u': goto yy53;
		default: goto yy3;
	}
yy13:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 'F': goto yy54;
		case 'N':
		case 'n': goto yy55;
		case 'f': goto yy56;
		default: goto yy3;
	}
yy14:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 'R': goto yy57;
		case 'r': goto yy58;
		default: goto yy3;
	}
yy15:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 0x00: goto yy59;
		case 'E': goto yy60;
		case 'e': goto yy61;
		default: goto yy3;
	}
yy16:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 'a': goto yy49;
		default: goto yy3;
	}
yy17:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 0x00: goto yy50;
		case 'o': goto yy51;
		case 'u': goto yy53;
		default: goto yy3;
	}
yy18:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 'f': goto yy56;
		case 'n': goto yy55;
		default: goto yy3;
	}
yy19:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 'r': goto yy58;
		default: goto yy3;
	}
yy20:
	yych = *(marker = ++cursor);
	switch (yych) {
		case 0x00: goto yy59;
		case 'e': goto yy61;
		default: goto yy3;
	}
yy21:
	yych = *++cursor;
	if (yych <= 0x00) goto yy1;
	goto yy3;
yy22:
	yych = *++cursor;
	switch (yych) {
		case '.': goto yy31;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy32;
		case 'E':
		case 'e': goto yy33;
		case 'I': goto yy34;
		case 'i': goto yy36;
		default: goto yy30;
	}
yy23:
	cursor = marker;
	goto yy3;
yy24:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy38;
		case 'x': goto yy44;
		default: goto yy40;
	}
yy25:
	yych = *++cursor;
yy26:
	switch (yych) {
		case 0x00: goto yy38;
		case ',':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy25;
		case '.': goto yy32;
		case ':': goto yy43;
		default: goto yy23;
	}
yy27:
	yych = *++cursor;
	switch (yych) {
		case '.': goto yy31;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy32;
		case 'E':
		case 'e': goto yy33;
		case 'I': goto yy64;
		case 'i': goto yy65;
		default: goto yy30;
	}
yy28:
	++cursor;
#line 73 "implicit.re"
	{   return "float#fix"; }
#line 319 "implicit.c"
yy29:
	yych = *++cursor;
yy30:
	switch (yych) {
		case 0x00: goto yy28;
		case ',':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy29;
		default: goto yy23;
	}
yy31:
	yych = *++cursor;
	switch (yych) {
		case '.':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy31;
		case 'E':
		case 'e': goto yy33;
		default: goto yy23;
	}
yy32:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy28;
		case ',': goto yy29;
		case '.': goto yy31;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy32;
		case 'E':
		case 'e': goto yy33;
		default: goto yy23;
	}
yy33:
	yych = *++cursor;
	switch (yych) {
		case '+':
		case '-': goto yy66;
		default: goto yy23;
	}
yy34:
	yych = *++cursor;
	switch (yych) {
		case 'N': goto yy67;
		case 'n': goto yy68;
		default: goto yy23;
	}
yy35:
	yych = *++cursor;
	switch (yych) {
		case 'A':
		case 'a': goto yy69;
		default: goto yy23;
	}
yy36:
	yych = *++cursor;
	switch (yych) {
		case 'n': goto yy68;
		default: goto yy23;
	}
yy37:
	yych = *++cursor;
	switch (yych) {
		case 'a': goto yy70;
		default: goto yy23;
	}
yy38:
	++cursor;
#line 71 "implicit.re"
	{   return "int"; }
#line 413 "implicit.c"
yy39:
	yych = *++cursor;
yy40:
	switch (yych) {
		case 0x00: goto yy71;
		case ',':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7': goto yy39;
		case '.': goto yy32;
		case '8':
		case '9': goto yy62;
		case ':': goto yy43;
		default: goto yy23;
	}
yy41:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7': goto yy72;
		case '8':
		case '9': goto yy73;
		default: goto yy40;
	}
yy42:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy73;
		default: goto yy63;
	}
yy43:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': goto yy74;
		case '6':
		case '7':
		case '8':
		case '9': goto yy75;
		default: goto yy23;
	}
yy44:
	yych = *++cursor;
	if (yych <= 0x00) goto yy23;
	goto yy77;
yy45:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy78;
		default: goto yy26;
	}
yy46:
	yych = *++cursor;
	if (yych <= 0x00) goto yy79;
	goto yy23;
yy47:
	++cursor;
#line 97 "implicit.re"
	{   return "default"; }
#line 506 "implicit.c"
yy48:
	yych = *++cursor;
	switch (yych) {
		case 'L': goto yy80;
		default: goto yy23;
	}
yy49:
	yych = *++cursor;
	switch (yych) {
		case 'l': goto yy81;
		default: goto yy23;
	}
yy50:
	++cursor;
#line 59 "implicit.re"
	{   return "bool#no"; }
#line 523 "implicit.c"
yy51:
	yych = *++cursor;
	if (yych <= 0x00) goto yy50;
	goto yy23;
yy52:
	yych = *++cursor;
	switch (yych) {
		case 'L': goto yy82;
		default: goto yy23;
	}
yy53:
	yych = *++cursor;
	switch (yych) {
		case 'l': goto yy83;
		default: goto yy23;
	}
yy54:
	yych = *++cursor;
	switch (yych) {
		case 'F': goto yy51;
		default: goto yy23;
	}
yy55:
	yych = *++cursor;
	if (yych <= 0x00) goto yy59;
	goto yy23;
yy56:
	yych = *++cursor;
	switch (yych) {
		case 'f': goto yy51;
		default: goto yy23;
	}
yy57:
	yych = *++cursor;
	switch (yych) {
		case 'U': goto yy84;
		default: goto yy23;
	}
yy58:
	yych = *++cursor;
	switch (yych) {
		case 'u': goto yy85;
		default: goto yy23;
	}
yy59:
	++cursor;
#line 57 "implicit.re"
	{   return "bool#yes"; }
#line 572 "implicit.c"
yy60:
	yych = *++cursor;
	switch (yych) {
		case 'S': goto yy55;
		default: goto yy23;
	}
yy61:
	yych = *++cursor;
	switch (yych) {
		case 's': goto yy55;
		default: goto yy23;
	}
yy62:
	yych = *++cursor;
yy63:
	switch (yych) {
		case ',':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy62;
		case '.': goto yy32;
		case ':': goto yy43;
		default: goto yy23;
	}
yy64:
	yych = *++cursor;
	switch (yych) {
		case 'N': goto yy86;
		case 'n': goto yy87;
		default: goto yy23;
	}
yy65:
	yych = *++cursor;
	switch (yych) {
		case 'n': goto yy87;
		default: goto yy23;
	}
yy66:
	yych = *++cursor;
	if (yych <= 0x00) goto yy23;
	goto yy89;
yy67:
	yych = *++cursor;
	switch (yych) {
		case 'F': goto yy90;
		default: goto yy23;
	}
yy68:
	yych = *++cursor;
	switch (yych) {
		case 'f': goto yy90;
		default: goto yy23;
	}
yy69:
	yych = *++cursor;
	switch (yych) {
		case 'N': goto yy91;
		default: goto yy23;
	}
yy70:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy92;
		case '.': goto yy93;
		case 'n': goto yy91;
		default: goto yy23;
	}
yy71:
	++cursor;
#line 65 "implicit.re"
	{   return "int#oct"; }
#line 651 "implicit.c"
yy72:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7': goto yy94;
		case '8':
		case '9': goto yy95;
		default: goto yy40;
	}
yy73:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy95;
		default: goto yy63;
	}
yy74:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy96;
		case '.': goto yy97;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy75;
		case ':': goto yy43;
		default: goto yy23;
	}
yy75:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy96;
		case '.': goto yy97;
		case ':': goto yy43;
		default: goto yy23;
	}
yy76:
	yych = *++cursor;
yy77:
	switch (yych) {
		case 0x00: goto yy98;
		case ',':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f': goto yy76;
		default: goto yy23;
	}
yy78:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy99;
		default: goto yy26;
	}
yy79:
	++cursor;
#line 99 "implicit.re"
	{   return "merge"; }
#line 757 "implicit.c"
yy80:
	yych = *++cursor;
	switch (yych) {
		case 'S': goto yy100;
		default: goto yy23;
	}
yy81:
	yych = *++cursor;
	switch (yych) {
		case 's': goto yy101;
		default: goto yy23;
	}
yy82:
	yych = *++cursor;
	switch (yych) {
		case 'L': goto yy102;
		default: goto yy23;
	}
yy83:
	yych = *++cursor;
	switch (yych) {
		case 'l': goto yy102;
		default: goto yy23;
	}
yy84:
	yych = *++cursor;
	switch (yych) {
		case 'E': goto yy55;
		default: goto yy23;
	}
yy85:
	yych = *++cursor;
	switch (yych) {
		case 'e': goto yy55;
		default: goto yy23;
	}
yy86:
	yych = *++cursor;
	switch (yych) {
		case 'F': goto yy103;
		default: goto yy23;
	}
yy87:
	yych = *++cursor;
	switch (yych) {
		case 'f': goto yy103;
		default: goto yy23;
	}
yy88:
	yych = *++cursor;
yy89:
	switch (yych) {
		case 0x00: goto yy104;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy88;
		default: goto yy23;
	}
yy90:
	yych = *++cursor;
	if (yych <= 0x00) goto yy105;
	goto yy23;
yy91:
	yych = *++cursor;
	if (yych <= 0x00) goto yy106;
	goto yy23;
yy92:
	++cursor;
#line 61 "implicit.re"
	{   return "bool#na"; }
#line 835 "implicit.c"
yy93:
	yych = *++cursor;
	switch (yych) {
		case 'c': goto yy107;
		case 'i': goto yy108;
		case 'r': goto yy109;
		default: goto yy23;
	}
yy94:
	yych = *++cursor;
	switch (yych) {
		case '-': goto yy110;
		default: goto yy40;
	}
yy95:
	yych = *++cursor;
	switch (yych) {
		case '-': goto yy110;
		default: goto yy63;
	}
yy96:
	++cursor;
#line 67 "implicit.re"
	{   return "int#base60"; }
#line 860 "implicit.c"
yy97:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy111;
		case ',':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy97;
		default: goto yy23;
	}
yy98:
	++cursor;
#line 63 "implicit.re"
	{   return "int#hex"; }
#line 882 "implicit.c"
yy99:
	yych = *++cursor;
	switch (yych) {
		case '-': goto yy110;
		default: goto yy26;
	}
yy100:
	yych = *++cursor;
	switch (yych) {
		case 'E': goto yy51;
		default: goto yy23;
	}
yy101:
	yych = *++cursor;
	switch (yych) {
		case 'e': goto yy51;
		default: goto yy23;
	}
yy102:
	yych = *++cursor;
	if (yych <= 0x00) goto yy1;
	goto yy23;
yy103:
	yych = *++cursor;
	if (yych <= 0x00) goto yy112;
	goto yy23;
yy104:
	++cursor;
#line 75 "implicit.re"
	{   return "float#exp"; }
#line 913 "implicit.c"
yy105:
	++cursor;
#line 79 "implicit.re"
	{   return "float#inf"; }
#line 918 "implicit.c"
yy106:
	++cursor;
#line 83 "implicit.re"
	{   return "float#nan"; }
#line 923 "implicit.c"
yy107:
	yych = *++cursor;
	switch (yych) {
		case 'h': goto yy113;
		default: goto yy23;
	}
yy108:
	yych = *++cursor;
	switch (yych) {
		case 'n': goto yy114;
		default: goto yy23;
	}
yy109:
	yych = *++cursor;
	switch (yych) {
		case 'e': goto yy115;
		default: goto yy23;
	}
yy110:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy116;
		default: goto yy23;
	}
yy111:
	++cursor;
#line 77 "implicit.re"
	{   return "float#base60"; }
#line 961 "implicit.c"
yy112:
	++cursor;
#line 81 "implicit.re"
	{   return "float#neginf"; }
#line 966 "implicit.c"
yy113:
	yych = *++cursor;
	switch (yych) {
		case 'a': goto yy117;
		default: goto yy23;
	}
yy114:
	yych = *++cursor;
	switch (yych) {
		case 't': goto yy118;
		default: goto yy23;
	}
yy115:
	yych = *++cursor;
	switch (yych) {
		case 'a': goto yy119;
		default: goto yy23;
	}
yy116:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy120;
		default: goto yy23;
	}
yy117:
	yych = *++cursor;
	switch (yych) {
		case 'r': goto yy121;
		default: goto yy23;
	}
yy118:
	yych = *++cursor;
	switch (yych) {
		case 'e': goto yy122;
		default: goto yy23;
	}
yy119:
	yych = *++cursor;
	switch (yych) {
		case 'l': goto yy123;
		default: goto yy23;
	}
yy120:
	yych = *++cursor;
	switch (yych) {
		case '-': goto yy124;
		default: goto yy23;
	}
yy121:
	yych = *++cursor;
	switch (yych) {
		case 'a': goto yy125;
		default: goto yy23;
	}
yy122:
	yych = *++cursor;
	switch (yych) {
		case 'g': goto yy126;
		default: goto yy23;
	}
yy123:
	yych = *++cursor;
	if (yych <= 0x00) goto yy127;
	goto yy23;
yy124:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy128;
		default: goto yy23;
	}
yy125:
	yych = *++cursor;
	switch (yych) {
		case 'c': goto yy129;
		default: goto yy23;
	}
yy126:
	yych = *++cursor;
	switch (yych) {
		case 'e': goto yy130;
		default: goto yy23;
	}
yy127:
	++cursor;
#line 85 "implicit.re"
	{   return "float#na"; }
#line 1071 "implicit.c"
yy128:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy131;
		default: goto yy23;
	}
yy129:
	yych = *++cursor;
	switch (yych) {
		case 't': goto yy132;
		default: goto yy23;
	}
yy130:
	yych = *++cursor;
	switch (yych) {
		case 'r': goto yy133;
		default: goto yy23;
	}
yy131:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy134;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy23;
		case 'T':
		case 't': goto yy137;
		default: goto yy136;
	}
yy132:
	yych = *++cursor;
	switch (yych) {
		case 'e': goto yy138;
		default: goto yy23;
	}
yy133:
	yych = *++cursor;
	if (yych <= 0x00) goto yy139;
	goto yy23;
yy134:
	++cursor;
#line 87 "implicit.re"
	{   return "timestamp#ymd"; }
#line 1131 "implicit.c"
yy135:
	yych = *++cursor;
yy136:
	switch (yych) {
		case '\t':
		case ' ': goto yy135;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy140;
		default: goto yy23;
	}
yy137:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy141;
		default: goto yy23;
	}
yy138:
	yych = *++cursor;
	switch (yych) {
		case 'r': goto yy142;
		default: goto yy23;
	}
yy139:
	++cursor;
#line 69 "implicit.re"
	{   return "int#na"; }
#line 1175 "implicit.c"
yy140:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy143;
		default: goto yy23;
	}
yy141:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy144;
		default: goto yy23;
	}
yy142:
	yych = *++cursor;
	if (yych <= 0x00) goto yy145;
	goto yy23;
yy143:
	yych = *++cursor;
	switch (yych) {
		case ':': goto yy146;
		default: goto yy23;
	}
yy144:
	yych = *++cursor;
	switch (yych) {
		case ':': goto yy147;
		default: goto yy23;
	}
yy145:
	++cursor;
#line 95 "implicit.re"
	{   return "str#na"; }
#line 1226 "implicit.c"
yy146:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy148;
		default: goto yy23;
	}
yy147:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy149;
		default: goto yy23;
	}
yy148:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy150;
		default: goto yy23;
	}
yy149:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy151;
		default: goto yy23;
	}
yy150:
	yych = *++cursor;
	switch (yych) {
		case ':': goto yy152;
		default: goto yy23;
	}
yy151:
	yych = *++cursor;
	switch (yych) {
		case ':': goto yy153;
		default: goto yy23;
	}
yy152:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy154;
		default: goto yy23;
	}
yy153:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy155;
		default: goto yy23;
	}
yy154:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy156;
		default: goto yy23;
	}
yy155:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy157;
		default: goto yy23;
	}
yy156:
	yych = *++cursor;
	switch (yych) {
		case '.': goto yy159;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy23;
		default: goto yy160;
	}
yy157:
	yych = *++cursor;
	switch (yych) {
		case '.': goto yy162;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy23;
		default: goto yy163;
	}
yy158:
	yych = *++cursor;
	switch (yych) {
		case '\t':
		case ' ': goto yy158;
		case '+':
		case '-': goto yy165;
		case 'Z': goto yy166;
		default: goto yy23;
	}
yy159:
	yych = *++cursor;
yy160:
	switch (yych) {
		case '\t':
		case ' ': goto yy158;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy159;
		default: goto yy23;
	}
yy161:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy167;
		default: goto yy23;
	}
yy162:
	yych = *++cursor;
yy163:
	switch (yych) {
		case '+':
		case '-': goto yy161;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy162;
		case 'Z': goto yy164;
		default: goto yy23;
	}
yy164:
	yych = *++cursor;
	if (yych <= 0x00) goto yy168;
	goto yy23;
yy165:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy169;
		default: goto yy23;
	}
yy166:
	yych = *++cursor;
	if (yych <= 0x00) goto yy170;
	goto yy23;
yy167:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy171;
		default: goto yy23;
	}
yy168:
	++cursor;
#line 89 "implicit.re"
	{   return "timestamp#iso8601"; }
#line 1495 "implicit.c"
yy169:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy172;
		default: goto yy23;
	}
yy170:
	++cursor;
#line 91 "implicit.re"
	{   return "timestamp#spaced"; }
#line 1515 "implicit.c"
yy171:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy168;
		case ':': goto yy173;
		default: goto yy23;
	}
yy172:
	yych = *++cursor;
	switch (yych) {
		case 0x00: goto yy170;
		case ':': goto yy174;
		default: goto yy23;
	}
yy173:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy175;
		default: goto yy23;
	}
yy174:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy176;
		default: goto yy23;
	}
yy175:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy164;
		default: goto yy23;
	}
yy176:
	yych = *++cursor;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy166;
		default: goto yy23;
	}
}
#line 103 "implicit.re"


}
