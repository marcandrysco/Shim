#ifndef STRING_SCAN_H
#define STRING_SCAN_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * string scanning function declarations
 */

int8_t str_getdigit(char ch);

unsigned int str_parse_uint(const char *str);

unsigned long str_toulong(const char *str, char **endptr);
bool str_endchk(const char *str);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
