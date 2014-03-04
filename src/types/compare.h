#ifndef TYPES_COMPARE_H
#define TYPES_COMPARE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * stock comparison function declarations
 */

int compare_str(const void *s1, const void *s2);
int compare_ptr(const void *p1, const void *p2);
int compare_int(const void *p1, const void *p2);
int compare_uint(const void *p1, const void *p2);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
