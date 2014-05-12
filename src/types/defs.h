#ifndef TYPES_DEFS_H
#define TYPES_DEFS_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Delete a reference.
 *   @ref: The reference.
 */

typedef void (*delete_f)(void *ref);

/**
 * Copy a reference.
 *   @ref: The source reference.
 *   &returns: The copied reference.
 */

typedef void *(*copy_f)(void *ref);


/**
 * Value comparison.
 *   @p1: The first pointer value.
 *   @p2: The second pointer value.
 *   &returns: Their order.
 */

typedef int (*compare_f)(const void *p1, const void *p2);


/**
 * Obtain the next iterator reference.
 *   @ref: The iterator reference.
 *   &returns: The reference. 'NULL' if at the end of the iterator.
 */

typedef void *(*iter_f)(void *ref);

/**
 * Iterator interface structure.
 *   @next: The next function.
 *   @delete: The internal deletion function.
 */

struct iter_i {
	iter_f next;
	delete_f delete;
};

/**
 * Iteator instance storage structure.
 *   @ref: The internal reference.
 *   @iface: The iterator interface.
 */

struct iter_t {
	void *ref;
	const struct iter_i *iface;
};


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


/*
 * common implementation variables
 */

extern struct iter_t iter_blank;

/*
 * blank function declarations
 */

void delete_noop(void *ref);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
