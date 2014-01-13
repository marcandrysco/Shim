#include "../common.h"
#include "base.h"


/*
 * implementation function declarations
 */

struct thread_t *_impl_thread_new(thread_f func, void *arg, const struct thread_attr_t *attr);
void _impl_thread_detach(struct thread_t *thread);
void *_impl_thread_join(struct thread_t *thread);

bool _impl_thread_once(struct thread_once_t *once, thread_once_f func);


/**
 * Create a new thread.
 *   @func: Thread function.
 *   @arg: The argument.
 *   @attr: Creation attributes.
 *   &returns: The thread.
 */

_export
struct thread_t *thread_new(thread_f func, void *arg, const struct thread_attr_t *attr)
{
	return _impl_thread_new(func, arg, attr);
}

/**
 * Join a detach.
 *   @thread: The thread.
 */

_export
void thread_detach(struct thread_t *thread)
{
	_impl_thread_detach(thread);
}

/**
 * Join a thread.
 *   @thread: The thread.
 *   &returns: The thread return value.
 */

_export
void *thread_join(struct thread_t *thread)
{
	return _impl_thread_join(thread);
}


/**
 * Run a function only once.
 *   @once: The once tracker.
 *   @func: The function.
 */

_export
void thread_once(struct thread_once_t *once, thread_once_f func)
{
	_impl_thread_once(once, func);
}
