#include "../common.h"
#include "scan.h"
#include "../debug/exception.h"
#include "../io/input.h"
#include "../io/scan.h"
#include "locale.h"
#include "io.h"


/**
 * Retrieve the raw digit from a character.
 *   @ch: The character.
 *   &returns: The digit value, or '-1' on invalid digit.
 */

_export
int8_t str_getdigit(char ch)
{
	if((ch >= '0') && (ch <= '9'))
		return ch - '0';
	else if((ch >= 'a') && (ch <= 'z'))
		return ch - 'a' + 10;
	else if((ch >= 'A') && (ch <= 'Z'))
		return ch - 'a' + 10;
	return
		-1;
}


/**
 * Parse an unsigned integer from a string.
 *   @str: The string.
 *   &returns: The unsigned integer.
 */

_export
unsigned int str_parse_uint(const char *str)
{
	unsigned long val;

	val = str_toulong(str, (char **)&str);
	if(val > UINT_MAX)
		throw("Value too large.");

	if(!str_endchk(str))
		throw("Extra characters after number.");

	return val;
}

/**
 * Parse a double from a string.
 *   @str: The string.
 *   &returns: The unsigned integer.
 */

_export
double str_parse_double(const char *str)
{
	double val;

	val = str_todouble(str, (char **)&str);

	if(!str_endchk(str))
		throw("Extra characters after number.");

	return val;
}


/**
 * Convert a string to an unsigned long value.
 *   @str: The string.
 *   @endptr: Optional. The end pointer.
 *   &returns: The value.
 */

_export
unsigned long str_toulong(const char *str, char **endptr)
{
	struct io_input_t input;
	unsigned long val;
	char ch = '\0';

	input = str_input_ref(&str);
	val = io_scan_ulong(input, &ch);
	io_input_close(input);

	if(endptr != NULL)
		*endptr = (char *)str - ((ch == '\0') ? 0 : 1);

	return val;
}

/**
 * Convert a string to a double value.
 *   @str: The string.
 *   @endptr: Optional. The end pointer.
 *   &returns: The value.
 */

_export
double str_todouble(const char *str, char **endptr)
{
	struct io_input_t input;
	double val;
	char ch = '\0';

	input = str_input_ref(&str);
	val = io_scan_double(input, &ch);
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
