#include "../common.h"
#include "iter.h"
#include "../mem/manage.h"
#include "defs.h"


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
 *   inner: The inner iterator.
 *   @func: The translation function.
 *   @arg: The argument.
 */

struct wrapper_t {
	struct iter_t inner;

	iter_wrapper_f func;
	void *arg;
};


/*
 * local function declarations
 */

static void *wrapper_next(struct wrapper_t *wrapper);
static void wrapper_delete(struct wrapper_t *wrapper);


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
