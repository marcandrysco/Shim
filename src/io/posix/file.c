#include "../../common.h"
#include "../defs.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include "../../debug/exception.h"
#include "../../debug/res.h"
#include "../../math/func.h"
#include "../../mem/base.h"
#include "../../mem/manage.h"


/**
 * File structure.
 *   @fd: The file descriptor.
 *   @opt: The open options.
 *   @buf: The data buffer.
 *   @idx, avail, nbytes: The buffer position, availability, and size.
 *   @op: The operation mode.
 */

struct file_t {
	int fd;
	enum io_file_e opt;

	uint8_t *buf;
	uint32_t idx, avail, nbytes;
	enum io_file_e op;
};

/*
 * local function declarations
 */

static size_t unbuf_read(struct file_t *file, void *restrict buf, size_t nbytes);
static size_t unbuf_write(struct file_t *file, const void *restrict buf, size_t nbytes);

static size_t buf_read(struct file_t *file, void *restrict buf, size_t nbytes);

static bool file_ctrl(struct file_t *file, enum io_ctrl_e cmd, void *data);
static void file_close(struct file_t *file);

/*
 * local variables
 */

static const struct io_file_i buf_iface = {
	{
		(io_ctrl_f)file_ctrl,
		(io_close_f)file_close
	},
	(io_read_f)buf_read,
	//(io_write_f)buf_write,
};

static const struct io_file_i unbuf_iface = {
	{
		(io_ctrl_f)file_ctrl,
		(io_close_f)file_close
	},
	(io_read_f)unbuf_read,
	(io_write_f)unbuf_write,
};


/**
 * Open a file.
 *   @path: The path.
 *   @opt: The access option.
 *   &returns: The input device.
 */

struct io_file_t _impl_io_file_open(const char *path, enum io_file_e opt)
{
	int fd, oflag;
	struct file_t *file;

	if((opt & io_read_e) && (opt & io_write_e))
		oflag = O_RDWR;
	else if(opt & io_read_e)
		oflag = O_RDONLY;
	else if(opt & io_write_e)
		oflag = O_WRONLY;
	else
		throw("Missing read/write option.");

	if(opt & io_append_e)
		oflag |= O_APPEND;

	if(opt & io_create_e)
		oflag |= O_CREAT;

	if(opt & io_trunc_e)
		oflag |= O_TRUNC;

	fd = open(path, oflag, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	file = mem_alloc(sizeof(struct file_t));
	file->fd = fd;
	file->opt = opt;
	file->op = 0;
	file->idx = 0;
	file->avail = 0;

	if(opt & io_unbuf_e) {
		file->nbytes = 0;
		file->buf = NULL;
	}
	else {
		file->nbytes = sysconf(_SC_PAGESIZE);
		file->buf = mem_alloc(file->nbytes);
	}

	return (struct io_file_t){ file, (file->opt & io_unbuf_e) ? &unbuf_iface : &buf_iface };
}


/**
 * Retrieve the current file position.
 *   @file: The file.
 *   &returns: The position.
 */

uint64_t _impl_io_file_tell(struct file_t *file)
{
	return lseek(file->fd, 0, SEEK_CUR);
}

/**
 * Retrieve the current file position.
 *   @file: The file.
 *   @offset: The offset.
 *   @whence: The position from which to seek.
 *   &returns: The new offset from the beginning of the file.
 */

uint64_t _impl_io_file_seek(struct file_t *file, int64_t offset, enum io_whence_e whence)
{
	int val;

	switch(whence) {
	case io_seek_set_e: val = SEEK_SET; break;
	case io_seek_cur_e: val = SEEK_CUR; break;
	case io_seek_end_e: val = SEEK_END; break;
	default: _fatal("Invalid seek type.");
	}

	return lseek(file->fd, offset, val);
}


/**
 * Flush the file buffer.
 *   @file: The file.
 *   &returns: True if the file was fully flushed, false otherwise.
 */

bool _impl_io_file_flush(struct file_t *file)
{
	void *buf;
	ssize_t nbytes, rem;

	if(file->op != io_write_e)
		return 0;

	buf = file->buf;
	rem = file->idx;

	while(rem > 0) {
		nbytes = write(file->fd, buf, rem);
		if(nbytes == 0)
			;
		if(nbytes < 0)
			throw("Failed to write to file.");

		buf += nbytes;
		rem -= nbytes;
	}

	file->idx = 0;

	return 0;
}


/**
 * Read from an unbuffered file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

static size_t unbuf_read(struct file_t *file, void *restrict buf, size_t nbytes)
{
	ssize_t ret;

	ret = read(file->fd, buf, nbytes);
	if(ret < 0)
		throw("Failed to read to file. %s.", strerror(errno));

	return ret;
}

/**
 * Write to an unbuffered file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

static size_t unbuf_write(struct file_t *file, const void *restrict buf, size_t nbytes)
{
	ssize_t ret;

	ret = write(file->fd, buf, nbytes);
	if(ret < 0)
		throw("Failed to write to file. %s.", strerror(errno));

	return ret;
}


static size_t buf_read(struct file_t *file, void *restrict buf, size_t nbytes)
{
	void *orig = buf;

	if(file->op == io_write_e) {
		if(!_impl_io_file_flush(file))
			return 0;
	}
	else if(file->op == io_read_e) {
		size_t size;

		size = m_sizemin(nbytes, file->avail);
		mem_copy(buf, file->buf + file->idx, size);
		file->idx += size;
		file->avail -= size;

		buf += size;
		nbytes -= size;
	}

	if(nbytes >= file->nbytes) {
		buf += unbuf_read(file, buf, nbytes);
		file->op = 0;
	}
	else {
		size_t size;

		file->avail = unbuf_read(file, file->buf, file->nbytes);

		size = m_sizemin(nbytes, file->avail);
		mem_copy(buf, file->buf, size);
		file->idx = size;
		file->avail -= size;

		buf += size;
	}

	return buf - orig;
}


/**
 * Write data to the file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @len: The length.
 *   &returns: The number of bytes written.
 */

size_t _impl_io_file_write(struct file_t *file, const void *restrict buf, size_t len)
{
	//if((file->idx + len) > file->len) {
		_impl_io_file_flush(file);

	//}
	return 0;
}

/**
 * Close the file.
 *   @file: The file.
 */

void _impl_io_file_close(struct file_t *file)
{
	if(file->buf != NULL) {
		_impl_io_file_flush(file);
		mem_free(file->buf);
	}

	close(file->fd);
	mem_free(file);
}


/**
 * Handle a control signal to a file.
 *   @file: The file.
 *   @cmd: The command.
 *   @data: The data.
 *   &returns: True if handled, false otherwise.
 */

static bool file_ctrl(struct file_t *file, enum io_ctrl_e cmd, void *data)
{
	return false;
}

/**
 * Close a file.
 *   @file: The file.
 */

static void file_close(struct file_t *file)
{
	if(!(file->opt & io_unbuf_e)) {
		mem_free(file->buf);
		//TODO: flush buffer for writes
	}

	mem_free(file);
}
