#include "../common.h"
#include "device.h"


/**
 * Blank control function.
 *   @ref: The reference.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: Always false.
 */

_export
bool io_blank_ctrl(void *ref, unsigned int id, void *data)
{
	return false;
}

/**
 * Blank device close function.
 *   @ref: The reference.
 */

_export
void io_blank_close(void *ref)
{
}
