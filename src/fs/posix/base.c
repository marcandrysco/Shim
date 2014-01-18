#include "../../common.h"
#include <unistd.h>


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
