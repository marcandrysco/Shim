#include "../../common.h"
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../../debug/exception.h"
#include "../../mem/manage.h"
#include "../../io/chunk.h"
#include "../../io/output.h"
#include "../../string/base.h"
#include "../../string/io.h"
#include "../../types/defs.h"


/*
 * local definitions
 */

#define NAMELEN 8

/*
 * local function declarations
 */

static void tmpfill(struct io_output_t output, void *arg);

static const char *lsdir_prev(DIR *dir);
static const char *lsdir_next(DIR *dir);
static void lsdir_delete(DIR *dir);

/*
 * local variables
 */

struct iter_i lsdir_iface = { (iter_next_f)lsdir_next, (iter_prev_f)lsdir_prev, (delete_f)lsdir_delete };

/*
 * global variables
 */

_export struct io_chunk_t fs_tmpchunk = { tmpfill, NULL };


/**
 * Retrieve the file size.
 *   @path: The path.
 *   &returns: The file size.
 */

uint64_t _impl_fs_size(const char *path)
{
	struct stat st;

	if(stat(path, &st) == -1)
		throw("Failed to stat file '%s'.", path);

	return st.st_size;
}

/**
 * Determine if a file path exists.
 *   @path: The path.
 *   &returns: True if it exists, false otherwise.
 */

_export
bool _impl_fs_exists(const char *path)
{
	return (access(path, F_OK) == 0) ? true : false;
}

/**
 * Determine if a path references a regular file.
 *   @path: The path.
 *   &returns: True if regular file, false otherwise.
 */

_export
bool _impl_fs_isfile(const char *path)
{
	struct stat info;

	if(stat(path, &info))
		throw("Failed to stat file '%s'.", path);

	return S_ISREG(info.st_mode);
}

/**
 * Determine if a path references a symbolic link.
 *   @path: The path.
 *   &returns: True if symbolic link, false otherwise.
 */

_export
bool _impl_fs_islink(const char *path)
{
	struct stat info;

	if(stat(path, &info))
		throw("Failed to stat file '%s'.", path);

	return S_ISLNK(info.st_mode);
}

/**
 * Determine if a path references a directory.
 *   @path: The path.
 *   &returns: True if directory, false otherwise.
 */

_export
bool _impl_fs_isdir(const char *path)
{
	struct stat info;

	if(stat(path, &info))
		throw("Failed to stat file '%s'.", path);

	return S_ISDIR(info.st_mode);
}


/**
 * Write data directly to a file.
 *   @path: The path.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 */

_export
void _impl_fs_write(const char *path, const void *buf, size_t nbytes)
{
	int fd;
	ssize_t ret;

	fd = open(path, O_WRONLY | O_CREAT, 0777);
	if(fd == -1)
		throw("Cannot open '%s' for writing. %s.", strerror(errno));

	do {
		ret = write(fd, buf, nbytes);
		if(ret <= 0)
			break;

		buf += ret;
		nbytes -= ret;
	} while(nbytes > 0);

	close(fd);
}

/**
 * Write data directly to a file.
 *   @path: The path.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 */

_export
void _impl_fs_writestr(const char *path, const char *str)
{
	_impl_fs_write(path, str, str_len(str));
}

/**
 * Read data directly from a file.
 *   @path: The path.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to read.
 */

_export
void _impl_fs_read(const char *path, void *buf, size_t nbytes)
{
	int fd;
	ssize_t ret;

	fd = open(path, O_RDONLY);
	if(fd == -1)
		throw("Cannot open '%s' for reading. %s.", strerror(errno));

	do {
		ret = read(fd, buf, nbytes);
		if(ret <= 0)
			break;

		buf += ret;
		nbytes -= ret;
	} while(nbytes > 0);

	close(fd);
}

/**
 * Read a string directly from a file.
 *   @path: The path.
 *   &returns: The allocated string.
 */

_export
char *_impl_fs_readstr(const char *path)
{
	char *str;
	uint64_t nbytes;

	nbytes = _impl_fs_size(path);
	str = mem_alloc(nbytes + 1);
	_impl_fs_read(path, str, nbytes);
	str[nbytes] = '\0';

	return str;
}

/**
 * Copy a file to a destination path. On failure, the destination may exist
 * but be incomplete.
 *   @dest: The destination path.
 *   @src: The source path.
 *   &returns: The number of bytes copied.
 */

_export
size_t _impl_fs_copy(const char *dest, const char *src)
{
	int dfd, sfd;
	size_t total = 0;
	ssize_t nbytes = 0, written = 0;
	uint8_t buf[16*1024], *ptr;

	dfd = open(dest, O_WRONLY | O_CREAT, 0777);
	sfd = open(src, O_RDONLY);

	if((dfd != -1) && (sfd != -1)) {
		while(true) {
			nbytes = read(sfd, buf, sizeof(buf));
			if(nbytes <= 0)
				break;

			ptr = buf;
			total += nbytes;

			do {
				written = write(dfd, ptr, nbytes);
				if(written < 0)
					break;

				ptr += written;
				nbytes -= written;
			} while(nbytes > 0);

			if(written < 0)
				break;
		}
	}

	if(dfd != -1)
		close(dfd);
	
	if(sfd != -1)
		close(sfd);

	if((dfd < 0) || (sfd < 0) || (nbytes < 0) || (written < 0))
		throw("Failed to copy files. %s.", strerror(errno));

	return total;
}


/**
 * Create a hard link.
 *   @dest: The link destination.
 *   @target: The target path.
 */

_export
void _impl_fs_link(const char *dest, const char *target)
{
	if(link(target, dest) < 0)
		throw("Failed to create hard link to '%s' from '%s'. %s.", target, dest, strerror(errno));
}

/**
 * Try to create a hard link.
 *   @dest: The link destination.
 *   @target: The target path.
 *   &returns: True if successful, false otherwise.
 */

_export
bool _impl_fs_trylink(const char *dest, const char *target)
{
	return link(target, dest) == 0;
}

/**
 * Create a symbolic link.
 *   @dest: The link destination.
 *   @target: The target path.
 */

_export
void _impl_fs_symlink(const char *dest, const char *target)
{
	if(symlink(target, dest) < 0)
		throw("Failed to create hard link to '%s' from '%s'. %s.", target, dest, strerror(errno));
}

/**
 * Try to create a symbolic link.
 *   @dest: The link destination.
 *   @target: The target path.
 */

_export
bool _impl_fs_trysymlink(const char *dest, const char *target)
{
	return symlink(target, dest) == 0;
}

/**
 * Create a directory.
 *   @path: The path.
 */

_export
void _impl_fs_mkdir(const char *path)
{
	if(mkdir(path, 0777) != 0)
		throw("Failed to create directory '%s'. %s.", strerror(errno));
}


/**
 * Remove a directory, failing if it is non-empty.
 *   @path: The path.
 */

_export
void _impl_fs_rmdir(const char *path)
{
	if(rmdir(path) != 0)
		throw("Failed to remove directory '%s'. %s.", strerror(errno));
}

/**
 * Remove a file.
 *   @path: The file path.
 */

_export
void _impl_fs_rmfile(const char *path)
{
	if(unlink(path) < 0)
		throw("Unable to remove file '%s'. %s.", path, strerror(errno));
}

/**
 * Recursively delete all files an directories from a path.
 *   @path: The path.
 */

_export
void _impl_fs_clear(const char *path)
{
	if(_impl_fs_isdir(path)) {
		DIR *dir;
		struct dirent *entry;

		dir = opendir(path);
		if(dir == NULL)
			throw("Unable to enumerate files in '%s'. %s.", path, strerror(errno));

		do {
			entry = readdir(dir);
			if(entry != NULL) {
				char sub[str_len(path) + str_len(entry->d_name) + 2];

				str_printf(sub, "%s/%s", path, entry->d_name);

				_impl_fs_clear(sub);
			}
		} while(str_isequal(entry->d_name, ".") || str_isequal(entry->d_name, ".."));

		closedir(dir);
		_impl_fs_rmdir(path);
	}
	else
		_impl_fs_rmfile(path);
}


/**
 * Retrieve the temporary directory.
 *   &returns: The temporary directory.
 */

_export
const char *_impl_fs_tmpdir()
{
	const char *path;

	path = getenv("TMPDIR");
	if(path == NULL)
		path = "/tmp";

	return path;
}


/**
 * Create a temporary directory.
 *   @prefix: Optional. The temporary path prefix.
 *   &returns: The allocted path.
 */

_export
char *_impl_fs_mktmpdir(const char *prefix)
{
	int err;
	char *path;

	if(prefix == NULL)
		prefix = _impl_fs_tmpdir();

	while(true) {
		path = mem_alloc(str_len(prefix) + NAMELEN + 1);
		str_printf(path, "%s%C", prefix, fs_tmpchunk);

		err = mkdir(path, 0700);
		if(err == 0)
			break;
		else if(err != EEXIST)
			throw("Failed to create a temporary directory. %s.", strerror(errno));

		mem_free(path);
	}

	return path;
}


/**
 * Fill in a temporary name for a output chunk.
 *   @output: The output.
 *   @arg: Unused argument.
 */

static void tmpfill(struct io_output_t output, void *arg)
{
	char buf[NAMELEN];
	unsigned int i, n;

	for(i = 0; i < NAMELEN; i++) {
		n = rand() % 62;
		if(n < 10)
			n = n + '0';
		else if(n < 36)
			n = n - 10 + 'a';
		else
			n = n - 36 + 'A';

		buf[i] = n;
	}

	io_output_write(output, buf, NAMELEN);
}


/**
 * Create an iterator over all entries in a directory.
 *   @path: The directory path.
 *   &returns: The iterator.
 */

_export
struct iter_t _impl_fs_lsdir(const char *path)
{
	DIR *dir;

	dir = opendir(path);
	if(dir == NULL)
		throw("Unable to enumerate files in '%s'. %s.", path, strerror(errno));

	return (struct iter_t){ dir, &lsdir_iface };
}

/**
 * Retrieve the previous file name from the directory iterator.
 *   @dir: The iterator.
 *   &returns: The name or null.
 */

static const char *lsdir_prev(DIR *dir)
{
	struct dirent *entry;

	do {
		seekdir(dir, telldir(dir) - 2);

		entry = readdir(dir);
		if(entry == NULL)
			return NULL;
	} while(str_isequal(entry->d_name, ".") || str_isequal(entry->d_name, ".."));

	return entry->d_name;
}

/**
 * Retrieve the next file name from the directory iterator.
 *   @dir: The iterator.
 *   &returns: The name or null.
 */

static const char *lsdir_next(DIR *dir)
{
	struct dirent *entry;

	do {
		entry = readdir(dir);
		if(entry == NULL)
			return NULL;
	} while(str_isequal(entry->d_name, ".") || str_isequal(entry->d_name, ".."));

	return entry->d_name;
}

/**
 * Close the directory iterator.
 *   @dir: The iterator.
 */

static void lsdir_delete(DIR *dir)
{
	closedir(dir);
}
