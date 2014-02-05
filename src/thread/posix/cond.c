#include "../../common.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "../../debug/exception.h"
#include "../../mem/manage.h"
#include "../cond.h"
#include "defs.h"


/**
 * Create a new condition variable.
 *   @attr: The condition variable attributes.
 *   &returns: The condition variable.
 */

struct thread_cond_t _impl_thread_cond_new(struct thread_cond_attr_t *attr)
{
	int err;
	struct thread_cond_t cond;

	err = pthread_cond_init(&cond.var, NULL);
	if(err != 0)
		throw("Failed to create condition variable. %s.", strerror(err));

	return cond;
}

/**
 * Delete a condition variable.
 *   @cond: The condition variable.
 */

void _impl_thread_cond_delete(struct thread_cond_t *cond)
{
	int err;

	err = pthread_cond_destroy(&cond->var);
	if(err != 0)
		throw("Failed to delete condition variable. %s.", strerror(err));
}


/**
 * Wait on a condition variable
 *   @cond: The condition variable.
 *   @mutex: The mutex.
 */

void _impl_thread_cond_wait(struct thread_cond_t *cond, struct thread_mutex_t *mutex)
{
	int err;

	err = pthread_cond_wait(&cond->var, &mutex->lock);
	if(err != 0)
		throw("Failed to wait on condition variable. %s.", strerror(err));
}

/**
 * Timed wait on a condition variable
 *   @cond: The condition variable.
 *   @mutex: The mutex.
 *   @time: The absolute time to wait until.
 *   &returns: True if successfully waited, false if wait timed out.
 */

bool _impl_thread_cond_timedwait(struct thread_cond_t *cond, struct thread_mutex_t *mutex, uint64_t utime)
{
	int err;
	struct timespec time = { utime / 1000000, 1000 * (utime % 1000000) };

	err = pthread_cond_timedwait(&cond->var, &mutex->lock, &time);
	if(err == ETIMEDOUT)
		return false;
	else if(err != 0)
		throw("Failed to wait on condition variable. %s.", strerror(err));

	return true;
}


/**
 * Signal the condition variable, waking up one waiting thread.
 *   @cond: The condition variable.
 */

void _impl_thread_cond_signal(struct thread_cond_t *cond)
{
	int err;

	err = pthread_cond_signal(&cond->var);
	if(err != 0)
		throw("Failed to signal condition variable. %s.", strerror(err));
}

/**
 * Broadcast to the condition variable, waking up all waiting thread.
 *   @cond: The condition variable.
 */

void _impl_thread_cond_broadcast(struct thread_cond_t *cond)
{
	int err;

	err = pthread_cond_broadcast(&cond->var);
	if(err != 0)
		throw("Failed to broadcast condition variable. %s.", strerror(err));
}
