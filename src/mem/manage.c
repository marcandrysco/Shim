#include "../common.h"
#include "manage.h"
#include "../debug/exception.h"
#include "../debug/res.h"
#include "../io/chunk.h"
#include "../io/print.h"


/*
 * implementation function declarations
 */

void *_impl_mem_alloc(size_t nbytes);
void *_impl_mem_realloc(void *ptr, size_t nbytes);
void _impl_mem_free(void *ptr);

/*
 * local function declarations
 */

static void dbg_chunk(struct io_output_t output, void *arg);


/**
 * Allocate memory.
 *   @nbytes: The number of bytes.
 */

_export
void *_mem_alloc(size_t nbytes)
{
	return _impl_mem_alloc(nbytes);
}

/**
 * Reallocate memory.
 *   @ptr: The original pointer.
 *   @nbytes: The number of bytes.
 */

_export
void *_mem_realloc(void *ptr, size_t nbytes)
{
	return _impl_mem_realloc(ptr, nbytes);
}

/**
 * Free memory.
 *   @ptr: The pointer.
 */

_export
void _mem_free(void *ptr)
{
	if(_debug) {
		if(ptr == NULL)
			_fatal("Attempt to free null pointer.");
	}

	_impl_mem_free(ptr);
}

/**
 * Delete memory only if it is not already NULL.
 *   @ptr: The pointer.
 */

_export
void _mem_delete(void *ptr)
{
	if(ptr != NULL)
		_mem_free(ptr);
}


/**
 * Debugging allocate memory.
 *   @nbytes: The number of bytes.
 *   @file: The file.
 *   @line: The line.
 *   &returns: The allocated memory.
 */

_export
void *_mem_alloc_dbg(size_t nbytes, const char *file, unsigned int line)
{
	void *ptr;

	ptr = _impl_mem_alloc(nbytes);
	if(ptr != NULL)
		_dbg_res_alloc(ptr, file, line, (struct io_chunk_t){ dbg_chunk, &nbytes });

	return ptr;
}

/**
 * Debugging allocate memory.
 *   @ptr: The original pointer.
 *   @nbytes: The number of bytes.
 *   @file: The file.
 *   @line: The line.
 *   &returns: The allocated memory.
 */

_export
void *_mem_realloc_dbg(void *ptr, size_t nbytes, const char *file, unsigned int line)
{
	if(ptr != NULL)
		_dbg_res_free(ptr);

	ptr = _impl_mem_realloc(ptr, nbytes);

	if(ptr != NULL)
		_dbg_res_alloc(ptr, file, line, (struct io_chunk_t){ dbg_chunk, &nbytes });

	return ptr;
}

/**
 * Debugging free memory.
 *   @ptr: The pointer.
 */

_export
void _mem_free_dbg(void *ptr)
{
	if(ptr != NULL)
		_dbg_res_free(ptr);

	_impl_mem_free(ptr);
}

/**
 * Debugging delete memory only if it is not already NULL.
 *   @ptr: The pointer.
 */

_export
void _mem_delete_dbg(void *ptr)
{
	if(ptr != NULL)
		_mem_free_dbg(ptr);
}


/**
 * Callback for printing out debugging information.
 *   @output: The output.
 *   @arg: The number of bytes as an argument.
 */

static void dbg_chunk(struct io_output_t output, void *arg)
{
	io_printf(output, "memory alloc, %u bytes", (unsigned int)*(size_t *)arg);
}
