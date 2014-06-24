#ifndef TYPES_FILTER_H
#define TYPES_FILTER_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * filter variables
 */

extern struct filter_t filter_blank;

/*
 * filter function declarations
 */

struct filter_t filter_new(filter_apply_f func, void *arg);


/**
 * Create the iterator from a filter.
 *   @filter: The filter.
 *   @in: The input.
 *   &returns: The output reference or null.
 */

static inline void *filter_apply(struct filter_t filter, void *in)
{
	return filter.iface->apply(filter.ref, in);
}

/**
 * Delete the filter.
 *   @filter: The filter.
 */

static inline void filter_delete(struct filter_t filter)
{
	filter.iface->delete(filter.ref);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
