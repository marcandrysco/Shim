#include "../common.h"
#include "cond.h"


/*
 * implementation function declarations
 */

struct thread_cond_t *_impl_thread_cond_new(struct thread_cond_attr_t *attr);
void _impl_thread_cond_delete(struct thread_cond_t *cond);

void _impl_thread_cond_wait(struct thread_cond_t *cond, struct thread_mutex_t *mutex);
bool _impl_thread_cond_timedwait(struct thread_cond_t *cond, struct thread_mutex_t *mutex, uint64_t utime);

void _impl_thread_cond_signal(struct thread_cond_t *cond);
void _impl_thread_cond_broadcast(struct thread_cond_t *cond);


/**
 * Create a new condition variable.
 *   @attr: The condition variable attributes.
 *   &returns: The condition variable.
 */

struct thread_cond_t *thread_cond_new(struct thread_cond_attr_t *attr)
{
	return _impl_thread_cond_new(attr);
}

/**
 * Delete a condition variable.
 *   @cond: The condition variable.
 */

void thread_cond_delete(struct thread_cond_t *cond)
{
	_impl_thread_cond_delete(cond);
}


/**
 * Wait on a condition variable
 *   @cond: The condition variable.
 *   @mutex: The mutex.
 */

void thread_cond_wait(struct thread_cond_t *cond, struct thread_mutex_t *mutex)
{
	_impl_thread_cond_wait(cond, mutex);
}

/**
 * Timed wait on a condition variable
 *   @cond: The condition variable.
 *   @mutex: The mutex.
 *   @time: The absolute time to wait until.
 *   &returns: True if successfully waited, false if wait timed out.
 */

bool thread_cond_timedwait(struct thread_cond_t *cond, struct thread_mutex_t *mutex, uint64_t utime)
{
	return _impl_thread_cond_timedwait(cond, mutex, utime);
}


/**
 * Signal the condition variable, waking up one waiting thread.
 *   @cond: The condition variable.
 */

void thread_cond_signal(struct thread_cond_t *cond)
{
	_impl_thread_cond_signal(cond);
}

/**
 * Broadcast to the condition variable, waking up all waiting thread.
 *   @cond: The condition variable.
 */

void thread_cond_broadcast(struct thread_cond_t *cond)
{
	_impl_thread_cond_broadcast(cond);
}
