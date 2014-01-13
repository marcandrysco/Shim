#include "common.h"


/*
 * local function declarations
 */

static void *sync_func(void *arg);


/**
 * Main entry point.
 *   @argc: The number of arguments.
 *   @argv: The argument array.
 *   &returns: The return status code.
 */

int main(int argc, char *argv[])
{
	unsigned int val = 0;
	struct thread_t *thread;

	printf("thread sync... ");
	thread = thread_new(sync_func, &val, NULL);
	if(thread_join(thread) != (void *)2)
		printf("failed\n"), sys_exit(1);
	else if(val != 5)
		printf("failed\n"), sys_exit(1);
	else
		printf("okay\n");

	return 0;
}


/**
 * Synchronization thread.
 *   @arg: The test intenger.
 *   &returns: Always two.
 */

static void *sync_func(void *arg)
{
	sys_usleep(10);
	*(unsigned int *)arg = 5;

	return (void *)2;
}
