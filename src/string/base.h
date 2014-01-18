#ifndef STRING_BASE_H
#define STRING_BASE_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * base string function declarations
 */

size_t str_len(const char *str);

void str_copy(char *restrict dest, const char *restrict src);
size_t str_ncopy(char *restrict dest, const char *restrict src, size_t nbytes);

bool str_isequal(const char *s1, const char *s2);
int str_cmp(const char *s1, const char *s2);

char *str_chr(const char *str, char ch);
char *str_pbrk(const char *str, const char *set);

char *str_dup(const char *str);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
