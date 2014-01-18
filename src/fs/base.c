#include "../common.h"
#include "base.h"


/*
 * implementation function declarations
 */

bool _impl_fs_exists(const char *path);



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
