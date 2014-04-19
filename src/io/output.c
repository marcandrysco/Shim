#include "../common.h"
#include "output.h"
#include "../debug/exception.h"
#include "../debug/res.h"
#include "../mem/manage.h"
#include "../io/chunk.h"
#include "../string/base.h"
#include "device.h"


/**
 * Counter structure.
 *   @nbytes: The number of bytes written.
 *
 *   @output: The output.
 */

struct counter_t {
	uint64_t *nbytes;
	struct io_output_t output;
};


/*
 * implementation function declarations
 */

struct io_output_t _impl_io_output_stdout();
struct io_output_t _impl_io_output_stderr();

struct io_output_t _impl_io_output_open(const char *path);
struct io_output_t _impl_io_output_open_dbg(const char *path, const char *file, unsigned int line);

struct io_output_t _impl_io_output_append(const char *path);
struct io_output_t _impl_io_output_append_dbg(const char *path, const char *file, unsigned int line);

/*
 * local function declarations
 */

static size_t counter_proc(struct counter_t *counter, const void *restrict buf, size_t nbytes);

/*
 * local variables
 */

static struct io_output_i counter_iface = { { io_blank_ctrl, mem_free }, (io_write_f)counter_proc };


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
 * Send a control request to the output device.
 *   @output: The output device.
 *   @cmd: The command.
 *   @arg: The argument.
 *   &returns: True if command handled, false otherwise.
 */

_export
bool io_output_ctrl(struct io_output_t output, unsigned int cmd, void *arg)
{
	return output.iface->device.ctrl(output.ref, cmd, arg);
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
 * Write a 16-bit unsigned integer to the output device.
 *   @output: The output device.
 *   @value: The value.
 */

_export
void io_output_uint16(struct io_output_t output, uint16_t value)
{
	io_output_writefull(output, &value, sizeof(uint16_t));
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
 * Write a 64-bit unsigned integer to the output device.
 *   @output: The output device.
 *   @value: The value.
 */

_export
void io_output_uint64(struct io_output_t output, uint64_t value)
{
	io_output_writefull(output, &value, sizeof(uint64_t));
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

/**
 * Write a string to the output device.
 *   @output: The output device.
 *   @str: The string.
 */

_export
void io_output_strptr(struct io_output_t output, const char *str)
{
	size_t len;

	if(str != NULL) {
		len = str_len(str);
		if(len > UINT32_MAX)
			throw("String too long to be written.");

		io_output_uint32(output, len);
		io_output_writefull(output, str, len);
	}
	else
		io_output_uint32(output, UINT32_MAX);
}

/**
 * Write a float to the output device.
 *   @output: The output device.
 *   @ch: The float.
 */

_export
void io_output_float(struct io_output_t output, float value)
{
	io_output_writefull(output, &value, sizeof(float));
}

/**
 * Write a double to the output device.
 *   @output: The output device.
 *   @ch: The double.
 */

_export
void io_output_double(struct io_output_t output, double value)
{
	io_output_writefull(output, &value, sizeof(double));
}


/**
 * Create an output counter.
 *   @output: The output.
 *   @nbytes: The number of bytes written.
 *   &returns: The output.
 */

_export
struct io_output_t io_output_counter(struct io_output_t output, uint64_t *nbytes)
{
	struct counter_t *counter;

	counter = mem_alloc(sizeof(struct counter_t));
	counter->nbytes = nbytes;
	counter->output = output;

	return (struct io_output_t){ counter, &counter_iface };
}

/**
 * Process the counter output.
 *   @counter: The counter.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

static size_t counter_proc(struct counter_t *counter, const void *restrict buf, size_t nbytes)
{
	nbytes = io_output_write(counter->output, buf, nbytes);
	*counter->nbytes += nbytes;

	return nbytes;
}
