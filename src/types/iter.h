#ifndef TYPES_ITER_H
#define TYPES_ITER_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Wrapper callback function.
 *   @ref: The input reference.
 *   @arg: The argument.
 *   &returns: The translated reference.
 */

typedef void *(*iter_wrapper_f)(void *ref, void *arg);


/*
 * iterator function declarations
 */

struct iter_t iter_wrapper(struct iter_t inner, iter_wrapper_f func, void *arg);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
