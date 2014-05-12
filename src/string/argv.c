#include "../common.h"
#include "../mem/manage.h"
#include "../types/strbuf.h"
#include "locale.h"


/**
 * Parse an argument array.
 *   @str: The input string.
 *   @argv: The output argument array.
 *   @argc: The output argument count.
 */

_export
void argv_parse(const char *str, char ***argv, int *argc)
{
	int n = 0;
	struct strbuf_t buf;

	*argv = mem_alloc(sizeof(char *));

	while(true) {
		while(str_isspace(*str))
			str++;

		if(*str == '\0')
			break;

		strbuf_init(&buf, 16);

		while(!str_isspace(*str) && *str != '\0')
			strbuf_store(&buf, *str++);

		(*argv)[n++] = strbuf_done(&buf);
		*argv = mem_realloc(*argv, (n + 1) * sizeof(char *));
	}

	(*argv)[n] = NULL;

	if(argc != NULL)
		*argc = n;
}

/**
 * Delete an argument array.
 *   @argv: The argument array.
 */

_export
void argv_delete(char **argv)
{
	char **arg;

	for(arg = argv; *arg != NULL; arg++)
		mem_free(*arg);

	mem_free(argv);
}
