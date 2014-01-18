#include "../common.h"
#include "scan.h"
#include "../io/input.h"
#include "../io/scan.h"
#include "locale.h"
#include "io.h"


_export
unsigned long str_toulong(const char *str, char **endptr)
{
	struct io_input_t input;
	unsigned long val;
	char ch;

	input = str_input_ref(&str);
	val = io_scan_ulong(input, &ch);
	io_input_close(input);

	if(endptr != NULL)
		*endptr = (char *)str - ((ch == '\0') ? 0 : 1);

	return val;
}

/**
 * Check a string to validate that only whitespace remains.
 *   @str: The string.
 *   &returns: True if only whiltespace remains, false otherwise.
 */

_export
bool str_endchk(const char *str)
{
	while(str_isspace(*str))
		str++;

	return *str == '\0';
}
