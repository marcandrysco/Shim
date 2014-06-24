#include "../common.h"
#include "filter.h"
#include "../mem/manage.h"


/**
 * Instance structure.
 *   @func: The function.
 *   @arg: The argument.
 */

struct inst_t {
	filter_f func;
	void *arg;
};


/*
 * local function declarations
 */

static void *inst_apply(struct inst_t *inst, void *in);

static void *blank_apply(void *ref, void *in);

/*
 * local variables
 */

static struct filter_i blank_iface = { blank_apply, delete_noop };

/*
 * global variables
 */

_export struct filter_t filter_blank = { NULL, &blank_iface };


/**
 * Create a new filter from a function and argument.
 *   @func: The function.
 *   @arg: The argument.
 *   &returns: The filter.
 */

_export
struct filter_t filter_new(filter_apply_f func, void *arg)
{
	struct inst_t *inst;
	static struct filter_i iface = { (filter_f)inst_apply, mem_free };

	inst = mem_alloc(sizeof(struct inst_t));
	inst->func = func;
	inst->arg = arg;

	return (struct filter_t){ inst, &iface };
}

/**
 * Creata an iterator from the filter instance.
 *   @inst: The instance.
 *   @in: The input.
 *   &returns: The output reference or null.
 */

static void *inst_apply(struct inst_t *inst, void *in)
{
	return inst->func(in, inst->arg);
}


/**
 * Generaete a blank iterator.
 *   @ref: Unusued reference.
 *   @in: The input.
 *   &returns: The output reference or null.
 */

static void *blank_apply(void *ref, void *in)
{
	return in;
}
