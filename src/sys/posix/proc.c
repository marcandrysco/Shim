#include "../../common.h"
#include <signal.h>
#include <stdlib.h>
#include "../../debug/exception.h"
#include "../../debug/res.h"
#include "../../mem/manage.h"
#include "../../thread/base.h"
#include "../../thread/host.h"
#include "../../types/llist.h"


/**
 * Exit handler instance.
 *   @func: The function.
 *   @node: The node.
 */

struct atexit_t {
	void (*func)();

	struct llist_node_t node;
};


/*
 * local function declarations
 */

static void atexit_reg();
static void atexit_handler();

static struct atexit_t *atexit_cast(struct llist_node_t *node);

/*
 * local variables
 */

static struct llist_root_t atexit_list;
static struct thread_once_t atexit_once = THREAD_ONCE_INIT;


/**
 * Exit the process, returning the given exit status.
 *   @status: The exit status.
 */

_noreturn
void _impl_sys_exit(int status)
{
	exit(status);
}


/**
 * Add a handler to execute when terminating the program.
 *   @func: The handler function.
 */

void _impl_sys_atexit(void (*func)())
{
	struct atexit_t *inst;

	thread_once(&atexit_once, atexit_reg);

	inst = mem_alloc(sizeof(struct atexit_t));
	inst->func = func;

	llist_root_append(&atexit_list, &inst->node);
}

/**
 * Exit handler initialization.
 */

void _impl_sys_atexit_init()
{
	thread_once(&atexit_once, atexit_reg);
}


static void signal_int()
{
	if(_debug)
		_backtrace();

	signal(SIGINT, NULL);
	raise(SIGINT);
}

static void signal_segv()
{
	atexit_handler();

	if(_debug)
		_backtrace();

	signal(SIGSEGV, NULL);
	raise(SIGSEGV);
}

/**
 * Register the atexit handler.
 */

static void atexit_reg()
{
	llist_root_init(&atexit_list);

	signal(SIGSEGV, signal_segv);
	signal(SIGINT, signal_int);

	atexit(_dbg_res_atexit);
	atexit(atexit_handler);
}

/**
 * Custom atexit handler to allow unlimited handlers.
 */

static void atexit_handler()
{
	struct atexit_t *inst;
	struct llist_node_t *cur, *next;

	for(cur = atexit_list.head; cur != NULL; cur = next) {
		next = cur->next;

		inst = atexit_cast(cur);
		inst->func();

		mem_free(inst);
	}
}


/**
 * Cast for atexit handler.
 *   @node: The node.
 *   &returns: The handler.
 */

static struct atexit_t *atexit_cast(struct llist_node_t *node)
{
	return (void *)node - offsetof(struct atexit_t, node);
}
