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

static struct iter_i iter_iface = { blank, noop };

/*
 * global variables
 */

_export struct iter_t iter_blank = { NULL, &iter_iface };


/**
 * No operation.
 *   @ref: Unused reference.
 */

static void noop(void *ref)
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

