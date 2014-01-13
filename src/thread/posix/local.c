#include "../../common.h"
#include <pthread.h>
#include "../../debug/exception.h"
#include "../../mem/manage.h"
#include "../local.h"


/**
 * Thread-local variable structure.
 *   @key: The key.
 */

struct thread_local_t {
	pthread_key_t key;
};


/**
 * Create the thread-local variable.
 *   @destroy: Optional. The destruction handler.
 *   &returns: The thread-local variable.
 */

struct thread_local_t *_impl_thread_local_new(void (*destroy)(void *))
{
	struct thread_local_t *local;

	local = mem_alloc(sizeof(struct thread_local_t));
	pthread_key_create(&local->key, destroy);

	return local;
}

/**
 * Delete the thread-local variable.
 *   @local: The thread-local variable.
 */

void _impl_thread_local_delete(struct thread_local_t *local)
{
	pthread_key_delete(local->key);
	mem_free(local);
}


/**
 * Get the thread-local value.
 *   @local: The thread-local variable.
 *   &returns: The value.
 */

void *_impl_thread_local_get(struct thread_local_t *local)
{
	return pthread_getspecific(local->key);
}

/**
 * Set the thread-local value.
 *   @local: The thread-local variable.
 *   &returns: The value.
 */

void _impl_thread_local_set(struct thread_local_t *local, void *value)
{
	pthread_setspecific(local->key, value);
}
