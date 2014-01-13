#include "../../common.h"
#include <pthread.h>
#include <string.h>
#include "../../debug/exception.h"
#include "../../mem/manage.h"
#include "../base.h"
#include "defs.h"


/**
 * Thread structure.
 *   @ref: Thread reference.
 */

struct thread_t {
	pthread_t ref;
};


/**
 * Create a new thread.
 *   @func: Thread function.
 *   @arg: The argument.
 *   @attr: Creation attributes.
 *   &returns: The thread.
 */

struct thread_t *_impl_thread_new(thread_f func, void *arg, const struct thread_attr_t *attr)
{
	int err;
	struct thread_t *thread;

	thread = mem_alloc(sizeof(struct thread_t));

	err = pthread_create(&thread->ref, NULL, func, arg);
	if(err != 0)
		throw("Unable to create thread. %s.", strerror(err));

	return thread;
}

/**
 * Deatach a thread.
 *   @thread: The thread.
 */

void _impl_thread_detach(struct thread_t *thread)
{
	int err;

	err = pthread_detach(thread->ref);
	if(err != 0)
		throw("Failed to detach thread. %s.", strerror(err));

	mem_free(thread);
}

/**
 * Join a thread.
 *   @thread: The thread.
 *   &returns: The thread return value.
 */

void *_impl_thread_join(struct thread_t *thread)
{
	void *retval;

	pthread_join(thread->ref, &retval);
	mem_free(thread);

	return retval;
}


/**
 * Single run a routine.
 *   @once: The once.
 *   @func: The function.
 */

void _impl_thread_once(struct thread_once_t *once, thread_once_f func)
{
	pthread_once(&once->once, func);
}
