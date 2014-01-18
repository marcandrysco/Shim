#include "../common.h"
#include "base.h"
#include <string.h>


/**
 * Copy memory.
 *   @dest: The destination.
 *   @src: The source.
 *   @nbytes: The number of bytes.
 */

_export
void mem_copy(void *restrict dest, const void *restrict src, size_t nbytes)
{
	memcpy(dest, src, nbytes);
}


/**
 * Compare two regions of memory to determine if they are equal.
 *   @p1: The first pointer.
 *   @p2: The second pointer.
 *   @nbyteS: The number of bytes to compare.
 *   &returns: True if equal, false otherwise.
 */

_export
bool mem_isequal(const void *restrict p1, const void *restrict p2, size_t nbytes)
{
	return !memcmp(p1, p2, nbytes);
}
