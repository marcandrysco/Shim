#include "../common.h"
#include "base.h"
#include <string.h>
#include "manage.h"


/**
 * Zero memory.
 *   @dest: The destination.
 *   @nbytes: The number of bytes to set.
 */

_export
void mem_zero(void *dest, size_t nbytes)
{
	memset(dest, 0x00, nbytes);
}

/**
 * Fill memory wil a given byte.
 *   @dest: The destination.
 *   @byte: The constant byte.
 *   @nbytes: The number of bytes to set.
 */

_export
void mem_set(void *dest, uint8_t byte, size_t nbytes)
{
	memset(dest, byte, nbytes);
}

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
 * Copy memory in possibly overlapping regions.
 *   @dest: The destination.
 *   @src: The source.
 *   @nbytes: The number of bytes.
 */

_export
void mem_move(void *dest, const void *src, size_t nbytes)
{
	memmove(dest, src, nbytes);
}


/**
 * Compare two regions of memory to determine if they are equal.
 *   @p1: The first pointer.
 *   @p2: The second pointer.
 *   @nbyteS: The number of bytes to compare.
 *   &returns: True if equal, false otherwise.
 */

_export
int mem_cmp(const void *p1, const void *p2, size_t nbytes)
{
	return memcmp(p1, p2, nbytes);
}

/**
 * Compare two regions of memory to determine if they are equal.
 *   @p1: The first pointer.
 *   @p2: The second pointer.
 *   @nbyteS: The number of bytes to compare.
 *   &returns: True if equal, false otherwise.
 */

_export
bool mem_isequal(const void *p1, const void *p2, size_t nbytes)
{
	return !memcmp(p1, p2, nbytes);
}


/**
 * Duplicate a chunk of memory.
 *   @ptr: The pointer.
 *   @nbytes: The number of bytes.
 *   &returns: The duplicated memory.
 */

_export
void *mem_dup(void *ptr, size_t nbytes)
{
	void *dup;

	dup = mem_alloc(nbytes);
	mem_copy(dup, ptr, nbytes);

	return dup;
}
