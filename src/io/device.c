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
