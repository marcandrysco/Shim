#include "../../common.h"
#include "../defs.h"
#include <stdio.h>
#include "../../debug/exception.h"
#include "../../debug/res.h"
#include "../../io/chunk.h"
#include "../../io/print.h"


/*
 * function declarations
 */

size_t _impl_io_output_file_write(FILE *file, const void *restrict buf, size_t nbytes);
void _impl_io_output_file_close(FILE *file);

/*
 * local function declarations
 */

static void res_chunk(struct io_output_t output, void *arg);

/*
 * global variables
 */

struct io_output_i _impl_io_output_file_iface = {
	{
		NULL,
		(io_close_f)_impl_io_output_file_close
	},
	(io_write_f)_impl_io_output_file_write
};


/**
 * Retrieve the standard output device.
 *   &returns: The standard output device.
 */

struct io_output_t _impl_io_output_stdout()
{
	return (struct io_output_t){ stdout, &_impl_io_output_file_iface };
}

/**
 * Retrieve the standard error device.
 *   &returns: The standard error device.
 */

struct io_output_t _impl_io_output_stderr()
{
	return (struct io_output_t){ stderr, &_impl_io_output_file_iface };
}


/**
 * Open an output device.
 *   @path: The path.
 *   &returns: The output device.
 */

struct io_output_t _impl_io_output_open(const char *path)
{
	FILE *file;

	file = fopen(path, "w");
	if(file == NULL)
		throw("Unable to open file '%s'.", path);

	return (struct io_output_t){ file, &_impl_io_output_file_iface };
}

/**
 * Open an output device with debugging information.
 *   @path: The path.
 *   &returns: The output device.
 */

struct io_output_t _impl_io_output_open_dbg(const char *path, const char *file, unsigned int line)
{
	struct io_output_t output;

	output = _impl_io_output_open(path);
	_dbg_res_alloc(output.ref, file, line, (struct io_chunk_t){ res_chunk, (void *)path });

	return output;
}


/**
 * Append to an output device.
 *   @path: The path.
 *   &returns: The output device.
 */

struct io_output_t _impl_io_output_append(const char *path)
{
	FILE *file;

	file = fopen(path, "a");
	if(file == NULL)
		throw("Unable to open file '%s'.", path);

	return (struct io_output_t){ file, &_impl_io_output_file_iface };
}

/**
 * Append to an output device with debugging information.
 *   @path: The path.
 *   &returns: The output device.
 */

struct io_output_t _impl_io_output_append_dbg(const char *path, const char *file, unsigned int line)
{
	struct io_output_t output;

	output = _impl_io_output_open(path);
	_dbg_res_alloc(output.ref, file, line, (struct io_chunk_t){ res_chunk, (void *)path });

	return output;
}


/**
 * Write to a buffered file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

size_t _impl_io_output_file_write(FILE *file, const void *restrict buf, size_t nbytes)
{
	return fwrite(buf, 1, nbytes, file);
}

/**
 * Close a buffered file.
 *   @file: The file.
 */

void _impl_io_output_file_close(FILE *file)
{
	if(_debug)
		_dbg_res_free(file);

	fclose(file);
}


/**
 * Callback for printing out debugging information.
 *   @output: The output.
 *   @arg: File path as argument.
 */

static void res_chunk(struct io_output_t output, void *arg)
{
	io_printf(output, "opened file, \"%s\"", (const char *)arg);
}
