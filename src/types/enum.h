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

struct enum_t enum_new(enum_f func, void *arg);


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
