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


/**
 * Enumerate callback function.
 *   @arg: The argument.
 *   &returns: The iterator.
 */

typedef struct iter_t (*iter_enum_f)(void *arg);

/**
 * Enuemrator handler structure.
 *   @func: The function.
 *   @arg: The argument.
 */

struct iter_enum_h {
	iter_enum_f func;
	void *arg;
};


/*
 * iterator variables
 */

extern struct iter_filter_h iter_filter_null;

/*
 * iterator function declarations
 */

struct iter_t iter_wrapper(struct iter_t inner, iter_wrapper_f func, void *arg);
struct iter_t iter_filter(struct iter_t iter, struct filter_t filter);
struct iter_t iter_enum(struct iter_enum_h handler);

/*
 * filter function declarations
 */


/**
 * Obtain the next iterator reference.
 *   @iter: The iterator.
 *   &returns: The reference. 'NULL' if at the end of the iterator.
 */

static inline void *iter_next(struct iter_t iter)
{
	return iter.iface->next(iter.ref);
}

/**
 * Delete the iterator.
 *   @iter: The iterator.
 */

static inline void iter_delete(struct iter_t iter)
{
	iter.iface->delete(iter.ref);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
