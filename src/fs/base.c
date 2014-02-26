#include "../common.h"
#include "base.h"


/*
 * implementation function declarations
 */

bool _impl_fs_exists(const char *path);
bool _impl_fs_isdir(const char *path);

void _impl_fs_mkdir(const char *path);
void _impl_fs_rmdir(const char *path);

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
 * Create a temporary directory.
 *   @prefix: Optional. The temporary path prefix.
 *   &returns: The allocted path.
 */

_export
char *fs_mktmpdir(const char *prefix)
{
	return _impl_fs_mktmpdir(prefix);
}
