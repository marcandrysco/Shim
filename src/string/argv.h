#ifndef STRING_ARGV_H
#define STRING_ARGV_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * argument vector function declarations
 */

void argv_parse(const char *str, char ***argv, int *argc);
void argv_delete(char **argv);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
