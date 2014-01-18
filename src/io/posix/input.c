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

bool _impl_io_input_file_ctrl(FILE *file, unsigned int cmd, void *arg);
size_t _impl_io_input_file_read(FILE *file, void *restrict buf, size_t nbytes);
void _impl_io_input_file_close(FILE *file);

/*
 * local function declarations
 */

static void res_chunk(struct io_output_t output, void *arg);

/*
 * global variables
 */

struct io_input_i _impl_io_input_file_iface = {
	{
		(io_ctrl_f)_impl_io_input_file_ctrl,
		(io_close_f)_impl_io_input_file_close
	},
	(io_read_f)_impl_io_input_file_read
};


/**
 * Retrieve the standard input device.
 *   &returns: The standard input device.
 */

struct io_input_t _impl_io_input_stdin()
{
	return (struct io_input_t){ stdin, &_impl_io_input_file_iface };
}


/**
 * Open an input device.
 *   @path: The path.
 *   &returns: The input device.
 */

struct io_input_t _impl_io_input_open(const char *path)
{
	FILE *file;

	file = fopen(path, "r");
	if(file == NULL)
		throw("Unable to open file '%s'.", path);

	return (struct io_input_t){ file, &_impl_io_input_file_iface };
}

/**
 * Open an input device with debugging information.
 *   @path: The path.
 *   &returns: The input device.
 */

struct io_input_t _impl_io_input_open_dbg(const char *path, const char *file, unsigned int line)
{
	struct io_input_t input;

	input = _impl_io_input_open(path);
	_dbg_res_alloc(input.ref, file, line, (struct io_chunk_t){ res_chunk, (void *)path });

	return input;
}


/**
 * Handle a control message to the file.
 *   @file: The file.
 *   @cmd: The command.
 *   @arg: The argument.
 *   &returns: True if command handled, false otherwise.
 */

bool _impl_io_input_file_ctrl(FILE *file, unsigned int cmd, void *arg)
{
	switch(cmd) {
	case IO_CTRL_EOS:
		*(bool *)arg = feof(file) ? true : false;
		break;

	default:
		return false;
	}

	return true;
}

/**
 * Read from a buffered file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

size_t _impl_io_input_file_read(FILE *file, void *restrict buf, size_t nbytes)
{
	return fread(buf, 1, nbytes, file);
}

/**
 * Close a buffered file.
 *   @file: The file.
 */

void _impl_io_input_file_close(FILE *file)
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
