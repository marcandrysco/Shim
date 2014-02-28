#include "../common.h"
#include "base.h"
#include "../types/defs.h"


/*
 * implementation function declarations
 */

bool _impl_fs_exists(const char *path);
bool _impl_fs_isdir(const char *path);

void _impl_fs_mkdir(const char *path);
void _impl_fs_rmdir(const char *path);
void _impl_fs_rmfile(const char *path);
void _impl_fs_clear(const char *path);

struct iter_t _impl_fs_lsdir(const char *path);

char *_impl_fs_mktmpdir(const char *prefix);


/**
 * Determine if a file path exists.
 *   @path: The path.
 *   &returns: True if it exists, false otherwise.
 */

_export
bool fs_exists(const char *path)
{
	return _impl_fs_exists(path);
}

/**
 * Determine if a path references a directory.
 *   @path: The path.
 *   &returns: True if directory, false otherwise.
 */

_export
bool fs_isdir(const char *path)
{
	return _impl_fs_isdir(path);
}


/**
 * Create a directory.
 *   @path: The path.
 */

_export
void fs_mkdir(const char *path)
{
	_impl_fs_mkdir(path);
}

/**
 * Remove a directory, failing if it is non-empty.
 *   @path: The path.
 */

_export
void fs_rmdir(const char *path)
{
	_impl_fs_rmdir(path);
}

/**
 * Remove a file.
 *   @path: The file path.
 */

_export
void fs_rmfile(const char *path)
{
	_impl_fs_rmfile(path);
}

/**
 * Recursively delete all files an directories from a path.
 *   @path: The path.
 */

_export
void fs_clear(const char *path)
{
	_impl_fs_clear(path);
}


/**
 * Create a temporary directory.
 *   @prefix: Optional. The temporary path prefix.
 *   &returns: The allocted path.
 */

_export
char *fs_mktmpdir(const char *prefix)
{
	return _impl_fs_mktmpdir(prefix);
}


/**
 * Create an iterator over all entries in a directory.
 *   @path: The directory path.
 *   &returns: The iterator.
 */

_export
struct iter_t fs_lsdir(const char *path)
{
	return _impl_fs_lsdir(path);
}
