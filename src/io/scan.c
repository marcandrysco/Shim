#include "../common.h"
#include "scan.h"
#include "../debug/exception.h"
#include "../types/strbuf.h"
#include "input.h"


static int8_t convdigit(char ch)
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

static char readdigit(struct io_input_t input)
{
	int16_t ch;

	ch = io_input_byte(input);
	return (ch == IO_EOS) ? '\0' : ch;
}


/**
 * Scan a string from the input.
 *   @input: The input.
 *   &returns: The allocated string.
 */

_export
char *io_scan_str(struct io_input_t input)
{
	int16_t byte;
	struct strbuf_t buf;

	strbuf_init(&buf, 32);

	while(true) {
		byte = io_input_byte(input);
		if((byte == '\n') || (byte == IO_EOS))
			break;

		strbuf_store(&buf, byte);
	}

	return strbuf_done(&buf);
}


/**
 * Scan an unsigned long from an input.
 *   @input: The input string.
 *   @ch: The buffered character.
 *   &returns: The unsigned long.
 */

_export
unsigned long io_scan_ulong(struct io_input_t input, char *ch)
{
	int8_t val, base = 10;
	unsigned long num = 0;

	if(*ch == '\0') {
		*ch = readdigit(input);
		if(*ch == '\0')
			throw("Unexpected end of input.");
	}

	if(!convdigit(*ch) == -1)
		throw("Invalid number.");

	if(*ch == '0') {
		*ch = readdigit(input);
		if(*ch == 'x') {
			base = 16;
			*ch = readdigit(input);
		}
		else if(*ch == 'b'){ 
			base = 2;
			*ch = readdigit(input);
		}
		else
			base = 8;
	}

	while((val = convdigit(*ch)) != -1) {
		if(val >= base)
			break;

		num = base * num + val;
		*ch = readdigit(input);
	}

	return num;
}
