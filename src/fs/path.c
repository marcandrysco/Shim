#include "../common.h"
#include "path.h"
#include "../string/base.h"


/**
 * Retrieve the base portion of the file paht.
 *   @path: The path.
 *   &returns: The name.
 */

_export
const char *fs_basename(const char *path)
{
	const char *str;

	str = str_rchr(path, '/');
	return str ? str + 1 : path;
}
