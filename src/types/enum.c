#include "../common.h"
#include "enum.h"


/*
 * local function declarations
 */

static struct iter_t blank_iter(void *ref);

/*
 * local variables
 */

static struct enum_i blank_iface = { blank_iter, delete_noop };

/*
 * global variables
 */

_export struct enum_t enum_blank = { NULL, &blank_iface };


/**
 * Generaete a blank iterator.
 *   @ref: Unusued reference.
 */

static struct iter_t blank_iter(void *ref)
{
	return iter_blank;
}
