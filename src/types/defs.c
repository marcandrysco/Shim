#include "../common.h"
#include "defs.h"


/*
 * local function declarations
 */

static void *blank(void *ref);

/*
 * iterator interface
 */

static struct iter_i iter_iface = { blank, delete_noop };

/*
 * global variables
 */

_export struct iter_t iter_blank = { NULL, &iter_iface };


/**
 * No-op delete operation.
 *   @ref: Unused reference.
 */

_export
void delete_noop(void *ref)
{
}


/**
 * Retrurn a blank element.
 *   @ref: Unused reference.
 *   &returns: Always null.
 */

static void *blank(void *ref)
{
	return NULL;
}

