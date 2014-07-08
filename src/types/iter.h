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
 * iterator variables
 */

extern struct iter_t iter_blank;

/*
 * iterator function declarations
 */

struct iter_t iter_new(void *ref, iter_f next, delete_f delete);
struct iter_t iter_wrapper(struct iter_t inner, iter_wrapper_f func, void *arg);
struct iter_t iter_filter(struct iter_t iter, struct filter_t filter);
struct iter_t iter_compose(struct iter_t outer, enum_f iter);


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
