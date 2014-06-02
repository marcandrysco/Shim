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
 * Wrapper callback function.
 *   @ref: The input reference.
 *   @arg: The argument.
 *   &returns: The translated reference or null to ignore.
 */

typedef void *(*iter_filter_f)(void *ref, void *arg);

/**
 * Filter handler structure.
 *   @func: The function.
 *   @arg: The argument.
 */

struct iter_filter_h {
	iter_filter_f func;
	void *arg;
};


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
struct iter_t iter_filter(struct iter_t inner, struct iter_filter_h filter);
struct iter_t iter_enum(struct iter_enum_h handler);


/**
 * Check if a filter is null.
 *   @filter: The filter.
 *   &returns: True if null, false otherwise.
 */

static inline bool iter_filter_isnull(struct iter_filter_h filter)
{
	return (filter.func == NULL) && (filter.arg == NULL);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
