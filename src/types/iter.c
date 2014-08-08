#include "../common.h"
#include "iter.h"
#include "../io/chunk.h"
#include "../mem/manage.h"
#include "../string/base.h"
#include "enum.h"
#include "filter.h"


/**
 * Dynamic instance structure.
 *   @ref: The reference.
 *   @next: The next callback.
 *   @delete: Deletion callback.
 */

struct inst_t {
	void *ref;
	iter_f next;
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

/**
 * Composed iterator structure.
 *   @inner, outer: The inner and our iterators.
 *   @iter: The inner enumerator.
 */

struct compose_t {
	struct iter_t inner, outer;

	enum_f iter;
};


/*
 * local function declarations
 */

static void *blank_next(void *ref);

static void *inst_next(struct inst_t *inst);
static void inst_delete(struct inst_t *inst);

static void *arr_next(void ***ptr);

static void *wrapper_next(struct wrapper_t *wrapper);
static void wrapper_delete(struct wrapper_t *wrapper);

static void *apply_next(struct apply_t *apply);
static void apply_delete(struct apply_t *apply);

static void *compose_next(struct compose_t *compose);
static void compose_delete(struct compose_t *compose);

/*
 * local variables
 */

static struct iter_i blank_iface = { blank_next, delete_noop };

/*
 * global variables
 */

_export struct iter_t iter_blank = { NULL, &blank_iface };


/**
 * Retrieve the next reference from a blank reference.
 *   @ref: The unused reference.
 *   &returns: NULL.
 */

static void *blank_next(void *ref)
{
	return NULL;
}


/**
 * Create a new iterator with given callbacks.
 *   @ref: The reference.
 *   @next: The next callback.
 *   @delete: Deletion callback.
 *   &returns: The iterator.
 */

_export
struct iter_t iter_new(void *ref, iter_f next, delete_f delete)
{
	struct inst_t *inst;
	static const struct iter_i iface = { (iter_f)inst_next, (delete_f)inst_delete };

	inst = mem_alloc(sizeof(struct inst_t));
	*inst = (struct inst_t){ ref, next, delete };

	return (struct iter_t){ inst, &iface };
}

/**
 * Retrieve the next reference from the instance.
 *   @inst: The instance.
 *   &returns: The reference.
 */

static void *inst_next(struct inst_t *inst)
{
	return inst->next(inst->ref);
}

/**
 * Delete the instance.
 *   @inst: The instance.
 */

static void inst_delete(struct inst_t *inst)
{
	inst->delete(inst->ref);
	mem_free(inst);
}


/**
 * Create an iterator on a constant array.
 *   @arr: The array data.
 *   &returns: The iterator.
 */

_export
struct iter_t iter_arr(void **arr)
{
	void ***ptr;
	static const struct iter_i iface = { (iter_f)arr_next, mem_free };

	ptr = mem_alloc(sizeof(void **));
	*ptr = arr;

	return (struct iter_t){ ptr, &iface };
}

/**
 * Create an iterator on an array of strings.
 *   @arr: The array data.
 *   &returns: The iterator.
 */

_export
struct iter_t iter_arrstr(const char *const *arr)
{
	return iter_arr((void **)arr);
}

/**
 * Retrieve the next value from the array iterator.
 *   @iter: The iterator.
 *   &returns: The value.
 */

static void *arr_next(void ***ptr)
{
	void *ref;

	ref = **ptr;
	if(ref != NULL)
		(*ptr)++;

	return ref;
}


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
	static struct iter_i iface = { (iter_f)wrapper_next, (delete_f)wrapper_delete };

	wrapper = mem_alloc(sizeof(struct wrapper_t));
	*wrapper = (struct wrapper_t){ inner, func, arg };

	return (struct iter_t){ wrapper, &iface };
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
 * Create an enumerator for composed iterators and enumerators.
 *   @outer: The outer iterator.
 *   @iter: The enumerator callback.
 *   &returns: The iterator.
 */

_export
struct iter_t iter_compose(struct iter_t outer, enum_f iter)
{
	struct compose_t *compose;
	static struct iter_i iface = { (iter_f)compose_next, (delete_f)compose_delete };

	compose = mem_alloc(sizeof(struct compose_t));
	compose->outer = outer;
	compose->inner = iter_blank;
	compose->iter = iter;

	return (struct iter_t){ compose, &iface };
}

/**
 * Retrieve the next reference from the composed iterator.
 *   @compose: The composition structure.
 *   &returns: The next reference or null.
 */

static void *compose_next(struct compose_t *compose)
{
	void *ref;

	while(true) {
		ref = iter_next(compose->inner);
		if(ref != NULL)
			return ref;

		ref = iter_next(compose->outer);
		if(ref == NULL)
			return NULL;

		compose->inner = compose->iter(ref);
	}
}

/**
 * Delete a composed iterator.
 *   @compose: The composition structure.
 */

static void compose_delete(struct compose_t *compose)
{
	iter_delete(compose->inner);
	iter_delete(compose->outer);
	mem_free(compose);
}
