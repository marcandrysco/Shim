#include "../common.h"
#include "defs.h"


/*
 * local function declarations
 */

static void noop(void *ref);

static void *blank(void *ref);

/*
 * iterator interface
 */

static struct iter_i iter_iface = { blank, blank, noop };

/*
 * global variables
 */

_export struct iter_t iter_blank = { NULL, &iter_iface };



static void noop(void *ref)
{
}

static void *blank(void *ref)
{
	return NULL;
}

