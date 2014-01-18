#include "../common.h"
#include "output.h"
#include "../debug/exception.h"
#include "../debug/res.h"
#include "../io/chunk.h"
#include "../string/base.h"

/*
struct io_output_t io_output_open(const char *path)
{

}
*/


/*
 * implementation function declarations
 */

struct io_output_t _impl_io_output_stdout();
struct io_output_t _impl_io_output_stderr();

struct io_output_t _impl_io_output_open(const char *path);
struct io_output_t _impl_io_output_open_dbg(const char *path, const char *file, unsigned int line);

struct io_output_t _impl_io_output_append(const char *path);
struct io_output_t _impl_io_output_append_dbg(const char *path, const char *file, unsigned int line);


/**
 * Retrieve the standard output.
 *   &returns: The standard output.
 */

_export
struct io_output_t io_output_stdout()
{
	return _impl_io_output_stdout();
}

/**
 * Retrieve the standard error output.
 *   &returns; The standard error output.
 */

_export
struct io_output_t io_output_stderr()
{
	return _impl_io_output_stderr();
}


/**
 * Open an output device from a path.
 *   @path: The path.
 *   &returns: The output path.
 */

_export
struct io_output_t _io_output_open(const char *path)
{
	return _impl_io_output_open(path);
}

/**
 * Open an output device from a path wtith debugging information.
 *   @path: The path.
 *   @file: The file.
 *   @line: The line.
 *   &returns: The output path.
 */

_export
struct io_output_t _io_output_open_dbg(const char *path, const char *file, unsigned int line)
{
	return _impl_io_output_open_dbg(path, file, line);
}

/**
 * Append to an output device from a path.
 *   @path: The path.
 *   &returns: The output path.
 */

_export
struct io_output_t _io_output_append(const char *path)
{
	return _impl_io_output_append(path);
}

/**
 * Append to an output device from a path wtith debugging information.
 *   @path: The path.
 *   @file: The file.
 *   @line: The line.
 *   &returns: The output path.
 */

_export
struct io_output_t _io_output_append_dbg(const char *path, const char *file, unsigned int line)
{
	return _impl_io_output_append_dbg(path, file, line);
}


/**
 * Write to the output device.
 *   @output: The output device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

_export
size_t io_output_write(struct io_output_t output, const void *restrict buf, size_t nbytes)
{
	return output.iface->write(output.ref, buf, nbytes);
}

/**
 * Write full chunk to the output device.
 *   @output: The output device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 */

_export
void io_output_writefull(struct io_output_t output, const void *restrict buf, size_t nbytes)
{
	size_t rem = nbytes;

	do {
		nbytes = io_output_write(output, buf, rem);
		if(nbytes == 0)
			throw("Unable to write data to output.");

		rem -= nbytes;
		buf += nbytes;
	} while(rem > 0);
}

/**
 * Close the output device.
 *   @output: The output device.
 */

_export
void io_output_close(struct io_output_t output)
{
	if(output.iface->device.close != NULL)
		output.iface->device.close(output.ref);
}


/**
 * Write a boolean to the output device.
 *   @output: The output device.
 *   @value: The value.
 */

_export
void io_output_bool(struct io_output_t output, bool value)
{
	io_output_uint8(output, value ? 1 : 0);
}

/**
 * Write an 8-bit unsigned integer to the output device.
 *   @output: The output device.
 *   @value: The value.
 */

_export
void io_output_uint8(struct io_output_t output, uint8_t value)
{
	io_output_writefull(output, &value, sizeof(uint8_t));
}

/**
 * Write a 32-bit unsigned integer to the output device.
 *   @output: The output device.
 *   @value: The value.
 */

_export
void io_output_uint32(struct io_output_t output, uint32_t value)
{
	io_output_writefull(output, &value, sizeof(uint32_t));
}

/**
 * Write a character to the output device.
 *   @output: The output device.
 *   @ch: The character.
 */

_export
void io_output_ch(struct io_output_t output, char ch)
{
	io_output_writefull(output, &ch, sizeof(char));
}

/**
 * Write a string to the output device.
 *   @output: The output device.
 *   @str: The string.
 */

_export
void io_output_str(struct io_output_t output, const char *str)
{
	size_t len;

	len = str_len(str);
	if(len > UINT32_MAX)
		throw("String too long to be written.");

	io_output_uint32(output, len);
	io_output_writefull(output, str, len);
}
