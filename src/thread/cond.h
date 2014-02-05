#ifndef THREAD_COND_H
#define THREAD_COND_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct thread_cond_t;
struct thread_cond_attr_t;
struct thread_mutex_t;

/*
 * condition variable function declarations
 */

struct thread_cond_t thread_cond_new(struct thread_cond_attr_t *attr);
void thread_cond_delete(struct thread_cond_t *cond);

void thread_cond_wait(struct thread_cond_t *cond, struct thread_mutex_t *mutex);
bool thread_cond_timedwait(struct thread_cond_t *cond, struct thread_mutex_t *mutex, uint64_t utime);

void thread_cond_signal(struct thread_cond_t *cond);
void thread_cond_broadcast(struct thread_cond_t *cond);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
