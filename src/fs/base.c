#include "../common.h"
#include "base.h"
#include "../types/defs.h"


/*
 * implementation function declarations
 */

bool _impl_fs_exists(const char *path);
bool _impl_fs_isfile(const char *path);
bool _impl_fs_islink(const char *path);
bool _impl_fs_isdir(const char *path);

void _impl_fs_write(const char *path, const void *buf, size_t nbytes);
void _impl_fs_writestr(const char *path, const char *str);
void _impl_fs_read(const char *path, void *buf, size_t nbytes);
char *_impl_fs_readstr(const char *path);
size_t _impl_fs_copy(const char *dest, const char *src);

void _impl_fs_link(const char *dest, const char *target);
bool _impl_fs_trylink(const char *dest, const char *target);
void _impl_fs_symlink(const char *dest, const char *target);
bool _impl_fs_trysymlink(const char *dest, const char *target);
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
 * Determine if a path references a regular file.
 *   @path: The path.
 *   &returns: True if regular file, false otherwise.
 */

_export
bool fs_isfile(const char *path)
{
	return _impl_fs_isfile(path);
}

/**
 * Determine if a path references a symbolic link.
 *   @path: The path.
 *   &returns: True if symbolic link, false otherwise.
 */

_export
bool fs_islink(const char *path)
{
	return _impl_fs_islink(path);
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
 * Write data directly to a file.
 *   @path: The path.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 */

_export
void fs_write(const char *path, const void *buf, size_t nbytes)
{
	_impl_fs_write(path, buf, nbytes);
}

/**
 * Write data directly to a file.
 *   @path: The path.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 */

_export
void fs_writestr(const char *path, const char *str)
{
	_impl_fs_writestr(path, str);
}

/**
 * Read data directly from a file.
 *   @path: The path.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to read.
 */

_export
void fs_read(const char *path, void *buf, size_t nbytes)
{
	_impl_fs_read(path, buf, nbytes);
}

/**
 * Read a string directly from a file.
 *   @path: The path.
 *   &returns: The allocated string.
 */

_export
char *fs_readstr(const char *path)
{
	return _impl_fs_readstr(path);
}

/**
 * Copy a file to a destination path. On failure, the destination may exist
 * but be incomplete.
 *   @dest: The destination path.
 *   @src: The source path.
 *   &returns: The number of bytes copied.
 */

_export
size_t fs_copy(const char *dest, const char *src)
{
	return _impl_fs_copy(dest, src);
}


/**
 * Create a hard link.
 *   @dest: The link destination.
 *   @target: The target path.
 */

_export
void fs_link(const char *dest, const char *target)
{
	_impl_fs_link(dest, target);
}

/**
 * Try to create a hard link.
 *   @dest: The link destination.
 *   @target: The target path.
 *   &returns: True if successful, false otherwise.
 */

_export
bool fs_trylink(const char *dest, const char *target)
{
	return _impl_fs_trylink(dest, target);
}

/**
 * Create a symbolic link.
 *   @dest: The link destination.
 *   @target: The target path.
 */

_export
void fs_symlink(const char *dest, const char *target)
{
	_impl_fs_symlink(dest, target);
}

/**
 * Try to create a symbolic link.
 *   @dest: The link destination.
 *   @target: The target path.
 */

_export
bool fs_trysymlink(const char *dest, const char *target)
{
	return _impl_fs_trysymlink(dest, target);
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
