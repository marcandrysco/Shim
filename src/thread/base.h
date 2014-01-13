#ifndef THREAD_BASE_H
#define THREAD_BASE_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct thread_t;
struct thread_attr_t;
struct thread_once_t;


/**
 * Thread function.
 *   @arg: The argument.
 *   &returns: The return value.
 */

typedef void *(*thread_f)(void *arg);

/**
 * Single run function.
 */

typedef void (*thread_once_f)();


/*
 * thread function declarations
 */

struct thread_t *thread_new(thread_f func, void *arg, const struct thread_attr_t *attr);
void thread_detach(struct thread_t *thread);
void *thread_join(struct thread_t *thread);

void thread_once(struct thread_once_t *once, thread_once_f func);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
