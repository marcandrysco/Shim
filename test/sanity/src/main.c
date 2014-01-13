#include "common.h"


/*
 * local function declarations
 */

static void *thread_func(void *arg);


/**
 * Main entry point.
 *   @argc: The number of arguments.
 *   @argv: The argument array.
 *   &returns: The return status code.
 */

int main(int argc, char *argv[])
{
	printf("printf... okay\n");

	printf("memory management... ");
	mem_free(mem_realloc(mem_alloc(10), 20));
	printf("okay\n");

	printf("spawn thread... ");
	thread_join(thread_new(thread_func, NULL, NULL));

	return 0;
}

/**
 * Thread test function.
 *   @arg: The argument.
 *   &returns: Always null.
 */

static void *thread_func(void *arg)
{
	printf("okay\n");

	return NULL;
}
