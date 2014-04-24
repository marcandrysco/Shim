#include "../common.h"
#include "file.h"
#include "../mem/base.h"
#include "../mem/manage.h"


/*
 * implementation function declarations
 */

struct io_file_t _impl_io_file_open(const char *path, enum io_file_e opt);

/*
 * local function declarations
 */

static bool input_ctrl(struct io_file_t *file, unsigned int id, void *arg);
static size_t input_read(struct io_file_t *file, void *restrict buf, size_t nbytes);

static bool output_ctrl(struct io_file_t *file, unsigned int id, void *arg);
static size_t output_write(struct io_file_t *file, const void *restrict buf, size_t nbytes);

/*
 * local variables
 */

static struct io_input_i input_iface = { { (io_ctrl_f)input_ctrl, mem_free }, (io_read_f)input_read };
static struct io_output_i output_iface = { { (io_ctrl_f)output_ctrl, mem_free }, (io_write_f)output_write };


/**
 * Open a file.
 *   @path: The path.
 *   @opt: The access option.
 *   &returns: The input device.
 */

_export
struct io_file_t _io_file_open(const char *path, enum io_file_e opt)
{
	return _impl_io_file_open(path, opt);
}

/**
 * Close the file.
 *   @file: The file.
 */

_export
void io_file_close(struct io_file_t file)
{
	file.iface->device.close(file.ref);
}


_export
bool io_file_ctrl(struct io_file_t file, unsigned int id, void *data)
{
	return file.iface->device.ctrl(file.ref, id, data);
}


/**
 * Read data from the file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

_export
size_t io_file_read(struct io_file_t file, void *restrict buf, size_t nbytes)
{
	return file.iface->read(file.ref, buf, nbytes);
}

/**
 * WRite data to the file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes write.
 */

_export
size_t io_file_write(struct io_file_t file, const void *restrict buf, size_t nbytes)
{
	return file.iface->write(file.ref, buf, nbytes);
}


/**
 * Retrieve the current file position.
 *   @file: The file.
 *   &returns: The position.
 */

_export
uint64_t io_file_tell(struct io_file_t file)
{
	return file.iface->tell(file.ref);
}

/**
 * Retrieve the current file position.
 *   @file: The file.
 *   @offset: The offset.
 *   @whence: The position from which to seek.
 *   &returns: The new offset from the beginning of the file.
 */

_export
uint64_t io_file_seek(struct io_file_t file, int64_t offset, enum io_whence_e whence)
{
	return file.iface->seek(file.ref, offset, whence);
}


/**
 * Create an input from a file.
 *   @file: The file.
 *   &returns: The input.
 */

_export
struct io_input_t io_file_input(struct io_file_t file)
{
	return (struct io_input_t){ mem_dup(&file, sizeof(struct io_file_t)), &input_iface };
}

/**
 * Send a control message to the input.
 *   @file: The file.
 *   @id: The identifer.
 *   @arg: The argument.
 *   &returns: Whether or not the message was handled.
 */

static bool input_ctrl(struct io_file_t *file, unsigned int id, void *arg)
{
	return io_file_ctrl(*file, id, arg);
}

/**
 * Write the input.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

static size_t input_read(struct io_file_t *file, void *restrict buf, size_t nbytes)
{
	return io_file_read(*file, buf, nbytes);
}


/**
 * Create an output from a file.
 *   @file: The file.
 *   &returns: The output.
 */

_export
struct io_output_t io_file_output(struct io_file_t file)
{
	return (struct io_output_t){ mem_dup(&file, sizeof(struct io_file_t)), &output_iface };
}

/**
 * Send a control message to the output.
 *   @file: The file.
 *   @id: The identifer.
 *   @arg: The argument.
 *   &returns: Whether or not the message was handled.
 */

static bool output_ctrl(struct io_file_t *file, unsigned int id, void *arg)
{
	return io_file_ctrl(*file, id, arg);
}

/**
 * Write the output.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

static size_t output_write(struct io_file_t *file, const void *restrict buf, size_t nbytes)
{
	return io_file_write(*file, buf, nbytes);
}
