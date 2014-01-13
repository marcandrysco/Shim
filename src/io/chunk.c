#include "../common.h"
#include "chunk.h"
#include "../string/base.h"
#include "../types/strbuf.h"
#include "output.h"
#include "print.h"


/*
 * implementation function declarations
 */

void *_impl_mem_alloc(size_t nbytes);
void _impl_mem_free(void *ptr);

/*
 * local function declarations
 */

static void str_proc(struct io_output_t output, void *arg);

/*
 * global variables
 */

struct io_chunk_t io_chunk_null = { NULL, NULL };


/**
 * Process an output chunk.
 *   @chunk: The chunk.
 *   @output: The output device.
 */

_export
void io_chunk_proc(struct io_chunk_t chunk, struct io_output_t output)
{
	chunk.func(output, chunk.arg);
}

/**
 * Process an output, storing and returning an allocated string.
 *   @chunk: The chunk.
 *   @output: The output device.
 */

_export
char *io_chunk_proc_str(struct io_chunk_t chunk)
{
	struct io_output_t output;
	struct strbuf_t buf;

	strbuf_init(&buf, 16);
	output = strbuf_output(&buf);
	io_chunk_proc(chunk, output);

	return strbuf_done(&buf);
}


/**
 * Create a string chunk.
 *   @str: The string.
 *   &returnS: The chunk.
 */

_export
struct io_chunk_t io_chunk_str(const char *str)
{
	return (struct io_chunk_t){ str_proc, (void *)str };
}

/**
 * Processing callback for string chunks.
 *   @output: The output.
 *   @arg: the argument.
 */

static void str_proc(struct io_output_t output, void *arg)
{
	io_print_str(output, (const char *)arg);
}


/**
 * Process an output object.
 *   @object: The object.
 *   @output: The output device.
 */

_export
void io_object_proc(struct io_object_t object, struct io_output_t output)
{
	object.iface->proc(object.ref, output);
}

/**
 * Close an output object.
 *   @object: The object.
 */

_export
void io_object_close(struct io_object_t object)
{
	object.iface->close(object.ref);
}
