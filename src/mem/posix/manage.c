#include "../../common.h"
#include <stdlib.h>


/**
 * Allocate memory.
 *   @nbytes: The number of bytes.
 */

void *_impl_mem_alloc(size_t nbytes)
{
	return malloc(nbytes);
}

/**
 * Reallocate memory.
 *   @ptr: The original pointer.
 *   @nbytes: The number of bytes.
 *   &returns: The reallocated pointer.
 */

void *_impl_mem_realloc(void *ptr, size_t nbytes)
{
	return realloc(ptr, nbytes);
}

/**
 * Free memory.
 *   @ptr: The pointer.
 */

void _impl_mem_free(void *ptr)
{
	free(ptr);
}
