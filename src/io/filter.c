#include "../common.h"
#include "filter.h"
#include "../mem/manage.h"
#include "chunk.h"


/**
 * Instance structure.
 *   @func: The function.
 *   @arg: The argument.
 */

struct inst_t {
	io_filter_apply_f func;
	void *arg;
};


/*
 * local function declarations
 */

static struct io_chunk_t inst_apply(struct inst_t *inst, void *in);

static struct io_chunk_t blank_apply(void *ref, void *in);

/*
 * local variables
 */

static struct io_filter_i blank_iface = { blank_apply, delete_noop };

/*
 * global variables
 */

_export struct io_filter_t io_filter_blank = { NULL, &blank_iface };


/**
 * Create a new filter from a function and argument.
 *   @func: The function.
 *   @arg: The argument.
 *   &returns: The filter.
 */

_export
struct io_filter_t io_filter_new(io_filter_apply_f func, void *arg)
{
	struct inst_t *inst;
	static struct io_filter_i iface = { (io_filter_f)inst_apply, mem_free };

	inst = mem_alloc(sizeof(struct inst_t));
	inst->func = func;
	inst->arg = arg;

	return (struct io_filter_t){ inst, &iface };
}

/**
 * Creata an iterator from the filter instance.
 *   @inst: The instance.
 *   @in: The input.
 *   &returns: The chunk.
 */

static struct io_chunk_t inst_apply(struct inst_t *inst, void *in)
{
	return inst->func(in, inst->arg);
}


/**
 * Generaete a blank iterator.
 *   @ref: Unusued reference.
 *   @in: The input.
 *   &returns: The chunk.
 */

static struct io_chunk_t blank_apply(void *ref, void *in)
{
	return io_chunk_null;
}
