#ifndef SYS_LIB_H
#define SYS_LIB_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct sys_dynlib_t;

/*
 * dynamic library variables
 */

extern const char sys_dynlib_ext[];

/*
 * dynamic library function declarations
 */

struct sys_dynlib_t *sys_dynlib_open(const char *path);
void sys_dynlib_close(struct sys_dynlib_t *dynlib);
void *sys_dynlib_symbol(struct sys_dynlib_t *dynlib, const char *restrict name);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
