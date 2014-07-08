#ifndef TYPES_ENUM_H
#define TYPES_ENUM_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * enumerator variables
 */

extern struct enum_t enum_blank;

/*
 * enumerator function declarations
 */

struct enum_t enum_new(void *ref, enum_f iter, delete_f delete);
struct enum_t enum_filter(struct enum_t inner, struct filter_t filter);
struct enum_t enum_sort(struct enum_t inner, compare_f compare);
struct enum_t enum_compose(struct enum_t outer, enum_f inner);


/**
 * Create the iterator from an enumerator.
 *   @enumer: The enumerator.
 *   &returns: The interator.
 */

static inline struct iter_t enum_iter(struct enum_t enumer)
{
	return enumer.iface->iter(enumer.ref);
}

/**
 * Delete the enumerator.
 *   @enumer: The enumerator.
 */

static inline void enum_delete(struct enum_t enumer)
{
	enumer.iface->delete(enumer.ref);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
