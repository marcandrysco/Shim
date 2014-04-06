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

static size_t len_write(size_t *len, const void *restrict buf, size_t nbytes);

static void str_proc(struct io_output_t output, void *arg);
static void strptr_proc(struct io_output_t output, void *arg);
static void cond_proc(struct io_output_t output, void *arg);
static void indent_proc(struct io_output_t output, void *arg);

/*
 * local variables
 */

static struct io_output_i len_iface = {
	{
		NULL,
		NULL
	},
	(io_write_f)len_write
};

/*
 * global variables
 */

_export struct io_chunk_t io_chunk_null = { NULL, NULL };


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
 * Process a chunk, retrieve the total length written.
 *   @chunk: The chunk.
 *   &returns: the total number of bytes written.
 */

_export
size_t io_chunk_proc_len(struct io_chunk_t chunk)
{
	size_t len;
	struct io_output_t output = { &len, &len_iface };

	io_chunk_proc(chunk, output);

	return len;
}

/**
 * Write callback for the length.
 *   @len: The length reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes to written.
 */

static size_t len_write(size_t *len, const void *restrict buf, size_t nbytes)
{
	*len += nbytes;

	return nbytes;
}


/**
 * Create a string chunk.
 *   @str: The string.
 *   &returns: The chunk.
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
	if(arg != NULL)
		io_print_str(output, (const char *)arg);
}

/**
 * Create a string pointer chunk.
 *   @str: The string pointer.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t io_chunk_strptr(char **str)
{
	return (struct io_chunk_t){ strptr_proc, (void *)str };
}

/**
 * Processing callback for string chunks.
 *   @output: The output.
 *   @arg: the argument.
 */

static void strptr_proc(struct io_output_t output, void *arg)
{
	io_print_str(output, *(char **)arg);
}

/**
 * Create a conditional chunk pair, processing the first if non-null,
 * otherwise processing the second.
 *   @pair: The pair.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t io_chunk_cond(const struct io_chunk_t *pair)
{
	return (struct io_chunk_t){ cond_proc, (void *)pair };
}

/**
 * Processing callback for conditional chunks.
 *   @output: The output.
 *   @arg: the argument.
 */

static void cond_proc(struct io_output_t output, void *arg)
{
	const struct io_chunk_t *pair = arg;

	io_chunk_proc(!io_chunk_isnull(pair[0]) ? pair[0] : pair[1], output);
}

/**
 * Create an indent chunk.
 *   @indent: The indent count.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t io_chunk_indent(const unsigned int *indent)
{
	return (struct io_chunk_t){ indent_proc, (void *)indent };
}

/**
 * Processing callback for indent.
 *   @output: The output.
 *   @arg: the argument.
 */

static void indent_proc(struct io_output_t output, void *arg)
{
	unsigned int n = *(unsigned int *)arg;
	static const char blank[8] = "        ";

	io_print_str(output, blank + (8 - n % 8));

	for(n /= 8; n != 0; n--)
		io_print_str(output, blank);
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
