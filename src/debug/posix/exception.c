#include "../../common.h"
#include <execinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


/**
 * Abort the process.
 */

_noreturn
void _impl_abort()
{
	abort();
}

/**
 * Backtrace function.
 */

_export
void _impl_backtrace()
{
	char **symbols = NULL;
	int i, nframe = 8;

	do {
		void *buffer[nframe];

		i = backtrace(buffer, nframe);
		if(i == nframe)
			symbols = backtrace_symbols(buffer, i);
		else
			nframe *= 2;
	} while(symbols == NULL);

	nframe = i;
	for(i = 0; i < nframe; i++)
		fprintf(stderr, "%s\n", symbols[i]);

	free(symbols);
}
