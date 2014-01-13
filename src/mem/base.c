#include "../common.h"
#include "base.h"
#include <string.h>


/**
 * Copy memory.
 *   @dest: The destination.
 *   @src: The source.
 *   @nbytes: The number of bytes.
 */

void mem_copy(void *restrict dest, const void *restrict src, size_t nbytes)
{
	memcpy(dest, src, nbytes);
}
