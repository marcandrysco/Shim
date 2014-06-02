#include "../common.h"
#include "iter.h"
#include "../io/chunk.h"
#include "../mem/manage.h"
#include "../string/base.h"
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

/**
 * Filter structure.
 *   inner: The inner iterator.
 *   @func: The translation function.
 *   @arg: The argument.
 */

struct filter_t {
	struct iter_t inner;

	iter_filter_f func;
	void *arg;
};


/*
 * local function declarations
 */

static void *wrapper_next(struct wrapper_t *wrapper);
static void wrapper_delete(struct wrapper_t *wrapper);

static void *filter_next(struct filter_t *filter);
static void filter_delete(struct filter_t *filter);

/*
 * global variables
 */

_export struct iter_filter_h iter_filter_null = { NULL, NULL };


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
 *   @filter: The filter handler.
 *   &returns: The iterator.
 */

_export
struct iter_t iter_filter(struct iter_t inner, struct iter_filter_h handler)
{
	struct filter_t *filter;
	static struct iter_i filter_iface = { (iter_f)filter_next, (delete_f)filter_delete };

	filter = mem_alloc(sizeof(struct filter_t));
	*filter = (struct filter_t){ inner, handler.func, handler.arg };

	return (struct iter_t){ filter, &filter_iface };
}

/**
 * Retrieve the next reference from the filter.
 *   @filter: The filter.
 *   &returns: The reference.
 */

static void *filter_next(struct filter_t *filter)
{
	void *ref;

	while((ref = iter_next(filter->inner)) != NULL) {
		ref = filter->func(ref, filter->arg);
		if(ref != NULL)
			return ref;
	}

	return NULL;
}

/**
 * Delete a iterator filter.
 *   @filter: THe filter.
 */

static void filter_delete(struct filter_t *filter)
{
	iter_delete(filter->inner);
	mem_free(filter);
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


static const char *filter_chunkstr(struct io_chunk_t *chunk, void *arg)
{
	char **buf = arg;

	if((chunk != NULL) && !io_chunk_isnull(*chunk))
		str_set(buf, io_chunk_proc_str(*chunk));
	else
		str_replace(buf, NULL);

	return *buf;
}

struct iter_filter_h iter_filter_chunkstr(char **buf)
{
	return (struct iter_filter_h){ (iter_filter_f)filter_chunkstr, buf };
}
