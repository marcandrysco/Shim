#include "../common.h"
#include "file.h"


/*
 * implementation function declarations
 */

struct io_file_t _impl_io_file_open(const char *path, enum io_file_e opt);


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
