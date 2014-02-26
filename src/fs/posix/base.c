#include "../../common.h"
#include <errno.h>
#include <dirent.h>
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


/*
 * local definitions
 */

#define NAMELEN 8

/*
 * local variables
 */

static void tmpfill(struct io_output_t output, void *arg);

/*
 * global variables
 */

_export struct io_chunk_t fs_tmpchunk = { tmpfill, NULL };


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
 * Determine if a path references a directory.
 *   @path: The path.
 *   &returns: True if directory, false otherwise.
 */

_export
bool _impl_fs_isdir(const char *path)
{
	struct stat info;

	stat(path, &info);

	return info.st_mode & S_IFDIR;
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
