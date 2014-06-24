#include "../common.h"
#include "iter.h"
#include "../io/chunk.h"
#include "../mem/manage.h"
#include "../string/base.h"
#include "filter.h"


/**
 * Dynamic instance structure.
 *   @ref: The reference.
 *   @func: The function.
 *   @delete: Deletion.
 */

struct inst_t {
	void *ref;
	iter_f func;
	delete_f delete;
};

/**
 * Wrapper structure.
 *   @inner: The inner iterator.
 *   @func: The translation function.
 *   @arg: The argument.
 */

struct wrapper_t {
	struct iter_t inner;

	iter_wrapper_f func;
	void *arg;
};

/**
 * Filter application structure.
 *   @iter: The inner iterator.
 *   @func: The application function.
 *   @arg: The argument.
 */

struct apply_t {
	struct iter_t iter;
	struct filter_t filter;
};


/*
 * local function declarations
 */

static void *wrapper_next(struct wrapper_t *wrapper);
static void wrapper_delete(struct wrapper_t *wrapper);

static void *apply_next(struct apply_t *apply);
static void apply_delete(struct apply_t *apply);

/*
 * global variables
 */


/**
 * Create a wrapper around an inner iterator.
 *   @inner: The inner iterator.
 *   @func: The translation function.
 *   @arg: The argument.
 */

_export
struct iter_t iter_wrapper(struct iter_t inner, iter_wrapper_f func, void *arg)
{
	struct wrapper_t *wrapper;
	static struct iter_i wrapper_iface = { (iter_f)wrapper_next, (delete_f)wrapper_delete };

	wrapper = mem_alloc(sizeof(struct wrapper_t));
	*wrapper = (struct wrapper_t){ inner, func, arg };

	return (struct iter_t){ wrapper, &wrapper_iface };
}

/**
 * Retrieve the next reference from the wrapper.
 *   @wrapper: The wrapper.
 *   &returns: The reference.
 */

static void *wrapper_next(struct wrapper_t *wrapper)
{
	return wrapper->func(iter_next(wrapper->inner), wrapper->arg);
}

/**
 * Delete a iterator wrapper.
 *   @wrapper: THe wrapper.
 */

static void wrapper_delete(struct wrapper_t *wrapper)
{
	iter_delete(wrapper->inner);
	mem_free(wrapper);
}


/**
 * Create a filter around an inner iterator.
 *   @inner: The inner iterator.
 *   @filter: The filter.
 *   &returns: The iterator.
 */

_export
struct iter_t iter_filter(struct iter_t iter, struct filter_t filter)
{
	struct apply_t *apply;
	static struct iter_i iface = { (iter_f)apply_next, (delete_f)apply_delete };

	apply = mem_alloc(sizeof(struct apply_t));
	*apply = (struct apply_t){ iter, filter };

	return (struct iter_t){ apply, &iface };
}

/**
 * Retrieve the next reference from the filter.
 *   @apply: The application structure.
 *   &returns: The reference.
 */

static void *apply_next(struct apply_t *apply)
{
	void *ref;

	while((ref = iter_next(apply->iter)) != NULL) {
		ref = filter_apply(apply->filter, ref);
		if(ref != NULL)
			return ref;
	}

	return NULL;
}

/**
 * Delete a iterator filter.
 *   @apply: The application structure.
 */

static void apply_delete(struct apply_t *apply)
{
	iter_delete(apply->iter);
	filter_delete(apply->filter);
	mem_free(apply);
}


/**
 * Create an iterator from an enumeration handler.
 *   @handler: The enumeration handler.
 *   &returns: The iterator.
 */

_export
struct iter_t iter_enum(struct iter_enum_h handler)
{
	return handler.func(handler.arg);
}
