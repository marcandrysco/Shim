#include "../../common.h"
#include <stdlib.h>


/**
 * Retrieve an environment value.
 *   @id: The identifer.
 *   &returns: The value or null.
 */

const char *_impl_os_env_get(const char *id)
{
	return getenv(id);
}

//void _impl_os_env_set(const char *id, struct io_chunk_t value)
//{
//}
