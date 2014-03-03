#include "../common.h"
#include "env.h"


/*
 * implementation function declarations
 */

const char *_impl_os_env_get(const char *id);


/**
 * Retrieve an environment value.
 *   @id: The identifer.
 *   &returns: The value or null.
 */

const char *os_env_get(const char *id)
{
	return _impl_os_env_get(id);
}
