#include "../common.h"
#include "local.h"


/*
 * implementation function declarations
 */

struct thread_local_t *_impl_thread_local_new(void (*destroy)(void *));
void _impl_thread_local_delete(struct thread_local_t *local);

void *_impl_thread_local_get(struct thread_local_t *local);
void _impl_thread_local_set(struct thread_local_t *local, void *value);


/**
 * Create the thread-local variable.
 *   @destroy: Optional. The destruction handler.
 *   &returns: The thread-local variable.
 */

struct thread_local_t *thread_local_new(void (*destroy)(void *))
{
	return _impl_thread_local_new(destroy);
}

/**
 * Delete the thread-local variable.
 *   @local: The thread-local variable.
 */

void thread_local_delete(struct thread_local_t *local)
{
	_impl_thread_local_delete(local);
}


/**
 * Get the thread-local value.
 *   @local: The thread-local variable.
 *   &returns: The value.
 */

void *thread_local_get(struct thread_local_t *local)
{
	return _impl_thread_local_get(local);
}

/**
 * Set the thread-local value.
 *   @local: The thread-local variable.
 *   &returns: The value.
 */

void thread_local_set(struct thread_local_t *local, void *value)
{
	_impl_thread_local_set(local, value);
}
