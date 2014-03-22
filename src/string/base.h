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
char *str_rchr(const char *str, char ch);
char *str_str(const char *str, char *sub);
char *str_pbrk(const char *str, const char *set);

void str_cat(char **restrict base, const char *restrict str);
char *str_dup(const char *str);
char *str_ndup(const char *str, size_t n);
char *str_clone(const char *str);
void str_replace(char **dest, const char *src);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
