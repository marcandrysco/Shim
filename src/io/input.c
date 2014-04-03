#include "../common.h"
#include "input.h"
#include "../debug/exception.h"
#include "../debug/res.h"
#include "../io/chunk.h"
#include "../mem/manage.h"
#include "../string/base.h"
#include "../types/strbuf.h"


/*
 * implementation function declarations
 */

struct io_input_t _impl_io_input_stdin();

struct io_input_t _impl_io_input_open(const char *path);
struct io_input_t _impl_io_input_open_dbg(const char *path, const char *file, unsigned int line);


/**
 * Retrieve the standard input.
 *   &returns: The standard input.
 */

_export
struct io_input_t io_input_stdin()
{
	return _impl_io_input_stdin();
}


/**
 * Open an input device from a path.
 *   @path: The path.
 *   &returns: The input path.
 */

_export
struct io_input_t _io_input_open(const char *path)
{
	return _impl_io_input_open(path);
}

/**
 * Open an input device from a path wtith debugging information.
 *   @path: The path.
 *   @file: The file.
 *   @line: The line.
 *   &returns: The input path.
 */

_export
struct io_input_t _io_input_open_dbg(const char *path, const char *file, unsigned int line)
{
	return _impl_io_input_open_dbg(path, file, line);
}


/**
 * Send a control missage to the input.
 *   @input: The input.
 *   @cmd: The command.
 *   @arg: The argument.
 */

_export
bool io_input_ctrl(struct io_input_t input, unsigned int cmd, void *arg)
{
	if(input.iface->device.ctrl != NULL)
		return input.iface->device.ctrl(input.ref, cmd, arg);
	else
		return false;
}

/**
 * Read from the input device.
 *   @input: The input device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

_export
size_t io_input_read(struct io_input_t input, void *restrict buf, size_t nbytes)
{
	return input.iface->read(input.ref, buf, nbytes);
}

/**
 * Input full chunk from the input device.
 *   @input: The input device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 */

_export
void io_input_readfull(struct io_input_t input, void *restrict buf, size_t nbytes)
{
	size_t rem = nbytes;

	do {
		nbytes = io_input_read(input, buf, rem);
		if(nbytes == 0)
			throw("Unable to read data from input.");

		rem -= nbytes;
		buf += nbytes;
	} while(rem > 0);
}

/**
 * Read a single byte, accounting for end-of-stream.
 *   @input: The input.
 *   &returns: The byte read or end-of-stream.
 */

_export
int16_t io_input_byte(struct io_input_t input)
{
	uint8_t byte;

	if(io_input_read(input, &byte, sizeof(uint8_t)) == 0)
		return IO_EOS;
	else
		return byte;
}

/**
 * Close the input device.
 *   @input: The input device.
 */

_export
void io_input_close(struct io_input_t input)
{
	if(input.iface->device.close != NULL)
		input.iface->device.close(input.ref);
}


/**
 * Probe the input device for the end-of-stream.
 *   @input: The input device.
 *   &returns: True if at the end-of-stream.
 */

_export
bool io_input_eos(struct io_input_t input)
{
	bool eos, suc;

	suc = io_input_ctrl(input, IO_CTRL_EOS, &eos);
	return suc && eos;
}


/**
 * Read a boolean from the input.
 *   @input: The input device.
 *   &returns: The value.
 */

_export
bool io_input_bool(struct io_input_t input)
{
	return io_input_uint8(input) ? true : false;
}

/**
 * Read an 8-bit unsigned integer from the input.
 *   @input: The input device.
 *   &returns: The value.
 */

_export
uint8_t io_input_uint8(struct io_input_t input)
{
	uint8_t value;

	io_input_readfull(input, &value, sizeof(uint8_t));

	return value;
}

/**
 * Read a 16-bit unsigned integer from the input.
 *   @input: The input device.
 *   &returns: The value.
 */

_export
uint16_t io_input_uint16(struct io_input_t input)
{
	uint16_t value;

	io_input_readfull(input, &value, sizeof(uint16_t));

	return value;
}

/**
 * Read a 32-bit unsigned integer from the input.
 *   @input: The input device.
 *   &returns: The value.
 */

_export
uint32_t io_input_uint32(struct io_input_t input)
{
	uint32_t value;

	io_input_readfull(input, &value, sizeof(uint32_t));

	return value;
}

/**
 * Read a 64-bit unsigned integer from the input.
 *   @input: The input device.
 *   &returns: The value.
 */

_export
uint64_t io_input_uint64(struct io_input_t input)
{
	uint64_t value;

	io_input_readfull(input, &value, sizeof(uint64_t));

	return value;
}

/**
 * Read a character from the input.
 *   @input: The input device.
 *   &returns: The character.
 */

_export
char io_input_ch(struct io_input_t input)
{
	char value;

	io_input_readfull(input, &value, sizeof(char));

	return value;
}

/**
 * Read a string from the input.
 *   @input: The input.
 *   &returns: The allocated string.
 */

_export
char *io_input_str(struct io_input_t input)
{
	char *str;
	uint32_t len;

	len = io_input_uint32(input);
	str = mem_alloc(len + 1);
	io_input_readfull(input, str, len);
	str[len] = '\0';

	return str;
}

/**
 * Read a string pointer from the input, the value which may be null.
 *   @input: The input.
 *   &returns: The allocated string or null.
 */

_export
char *io_input_strptr(struct io_input_t input)
{
	char *str;
	uint32_t len;

	len = io_input_uint32(input);
	if(len != UINT32_MAX) {
		str = mem_alloc(len + 1);
		io_input_readfull(input, str, len);
		str[len] = '\0';
	}
	else
		str = NULL;

	return str;
}

/**
 * Input a line of text, delimited by a newline or EOS.
 *   @input: The input.
 *   &returns; The line.
 */

_export
char *io_input_line(struct io_input_t input)
{
	char *line;
	int16_t ch;
	struct strbuf_t buf;

	buf = strbuf_empty(64);
	do {
		ch = io_input_byte(input);
		if(ch == IO_EOS)
			break;

		strbuf_store(&buf, ch);
	} while(ch != '\n');

	line = strbuf_done(&buf);
	if(*line == '\0') {
		mem_free(line);

		return NULL;
	}
	else
		return line;
}

/**
 * Read a float from the input.
 *   @input: The input device.
 *   &returns: The float.
 */

_export
float io_input_float(struct io_input_t input)
{
	float value;

	io_input_readfull(input, &value, sizeof(float));

	return value;
}

/**
 * Read a double from the input.
 *   @input: The input device.
 *   &returns: The double.
 */

_export
double io_input_double(struct io_input_t input)
{
	double value;

	io_input_readfull(input, &value, sizeof(double));

	return value;
}
