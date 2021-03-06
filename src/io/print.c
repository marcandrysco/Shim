#include "../common.h"
#include "print.h"
#include "../debug/exception.h"
#include "../io/chunk.h"
#include "../math/func.h"
#include "../mem/base.h"
#include "../string/base.h"
#include "../string/locale.h"
#include "output.h"


/*
 * global variables
 */

struct io_print_t io_print_default[] = {
	{ 's', "string", 0, io_printf_str },
	{ 'd', "decimal", 0, io_printf_int },
	{ 'i', "integer", 0, io_printf_int },
	{ 'u', "unsigned", 0, io_printf_uint },
	{ 'x', "hex", 0, io_printf_hex },
	{ 'b', "binary", 0, io_printf_binary },
	{ 'u', "unsigned", 0, io_printf_uint },
	{ 'c', "char", 0, io_printf_char },
	{ 'C', "chunk", 0, io_printf_chunk },
	{ 'f', "float", 0, io_printf_float },
	{ '\0', NULL, 0, NULL }
};


/**
 * Print out a string.
 *   @output: The output device.
 *   @string: The string.
 */

_export
void io_print_(struct io_output_t output, const char *str)
{
	io_output_write(output, str, str_len(str));
}

/**
 * Print out a string.
 *   @output: The output device.
 *   @string: The string.
 */

_export
void io_print_str(struct io_output_t output, const char *str)
{
	io_output_write(output, str, str_len(str));
}

/**
 * Print out a signed integer.
 *   @output: The output device.
 *   @value: The value.
 */

_export
void io_print_int(struct io_output_t output, int value)
{
	uint8_t i = 0;
	char buf[20];
	bool neg = value < 0;
	unsigned int uval = (value >= 0) ? value : -value;

	do
		buf[20 - ++i] = (uval % 10) + '0';
	while((uval /= 10) > 0);

	if(neg)
		buf[20 - ++i] = '-';

	io_output_write(output, buf + 20 - i, i);
}

/**
 * Print out an unsigned integer.
 *   @output: The output device.
 *   @value: The value.
 */

_export
void io_print_uint(struct io_output_t output, unsigned int value)
{
	uint8_t i = 0;
	char buf[19];

	do
		buf[19 - ++i] = (value % 10) + '0';
	while((value /= 10) > 0);

	io_output_write(output, buf + 19 - i, i);
}


/**
 * Print a formatted list.
 *   @output: The output the device.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 */

_export
void io_printf(struct io_output_t output, const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	io_vprintf(output, format, args);
	va_end(args);
}

/**
 * Print a variable arugment formatted list.
 *   @output: The output the device.
 *   @format: The print-style format.
 *   @args: The print-style arguments.
 */

_export
void io_vprintf(struct io_output_t output, const char *restrict format, va_list args)
{
	struct arglist_t list;

	va_copy(list.args, args);
	io_vprintf_custom(output, io_print_default, format, &list);
	va_end(list.args);
}

/**
 * Print a formatted list using custom callacks.
 *   @output: The output the device.
 *   @print: The print callback table.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 */

_export
void io_vprintf_custom(struct io_output_t output, struct io_print_t *print, const char *format, struct arglist_t *args)
{
	size_t i = 0;
	struct io_print_t *search;
	struct io_print_mod_t mod;
	char name[16];

	while(*format != '\0') {
		if(*format == '%') {
			format++;

			if(*format != '%') {
				if(*format == '-')
					mod.neg = true, format++;
				else
					mod.neg = false;

				if(*format == '0')
					mod.zero = true, format++;
				else
					mod.zero = false;

				mod.width = 0;
				while(str_isdigit(*format))
					mod.width = mod.width * 10 + *format - '0', format++;

				if(*format == ':') {
					format++;

					for(i = 0; format[i] != ':'; i++) {
						if(i == 15)
							throw("Format name too long.");
						else if(format[i] == '\0')
							throw("Invalid format, no ending ':'.");
					}

					mem_copy(name, format, i);
					name[i] = '\0';

					search = print;
					while(1) {
						if(search->callback == NULL)
							throw("Invalid token '%c'.", *format);
						if((search->name != NULL) && str_isequal(search->name, name))
							break;

						search++;
					}

					format += i;
				}
				else {
					search = print;
					while(search->ch != *format) {
						if(search->callback == NULL)
							throw("Invalid token '%c'.", *format);

						search++;
					}
				}

				search->callback(output, &mod, args);
			}
			else 
				io_output_ch(output, '%');

			format++;
		}
		else {
			i = 0;
			do
				i++;
			while((format[i] != '%') && (format[i] != '\0'));

			io_output_write(output, format, i);
			format += i;
		}
	}
}


/**
 * Printf-style chunk output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_chunk(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_chunk(output, va_arg(list->args, struct io_chunk_t), mod->width, mod->neg, ' ');
}

/**
 * Printf-style string output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */
_export
void io_printf_str(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_str(output, va_arg(list->args, const char *), mod->width, mod->neg, ' ');
}

/**
 * Printf-style integer output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_int(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_int(output, va_arg(list->args, int), 10, mod->width, mod->zero);
}

/**
 * Printf-style unsigned integer output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_uint(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_uint(output, va_arg(list->args, unsigned int), 10, mod->width, mod->neg, mod->zero);
}

/**
 * Printf-style unsigned integer output in hexidecimal.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_hex(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_uint(output, va_arg(list->args, unsigned int), 16, mod->width, mod->neg, mod->zero);
}

/**
 * Printf-style unsigned integer output in binary.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_binary(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_uint(output, va_arg(list->args, unsigned int), 2, mod->width, mod->neg, mod->zero);
}

/**
 * Printf-style character output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_char(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	uint8_t ch;

	ch = va_arg(list->args, int);
	io_output_write(output, &ch, sizeof(uint8_t));
}

/**
 * Printf-style float output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

int sprintf(const char *restrict, const char *restrict, ...);
_export
void io_printf_float(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	char format[10], str[20];

	if(mod->width)
		sprintf(format, "%%0%dg", mod->width);
	else
		sprintf(format, "%%g");

	sprintf(str, format, va_arg(list->args, double));
	io_print_str(output, str);
}


/**
 * Format a chunk.
 *   @output: The output device.
 *   @chunk: The chunk.
 *   @width: The width.
 *   @neg: Negative alignment.
 *   @pad: Padding character.
 */

_export
void io_format_chunk(struct io_output_t output, struct io_chunk_t chunk, uint16_t width, bool neg, char pad)
{
	if(width > 0) {
		size_t len = io_chunk_proc_len(chunk);
		if(width > len) {
			uint16_t i;

			if(neg)
				io_chunk_proc(chunk, output);

			for(i = len; i < width; i++)
				io_output_ch(output, pad);

			if(!neg)
				io_chunk_proc(chunk, output);
		}
		else
			io_chunk_proc(chunk, output);
	}
	else
		io_chunk_proc(chunk, output);
}

/**
 * Format a string.
 *   @output: The output device.
 *   @str: The string.
 *   @width: The width.
 *   @neg: Negative alignment.
 *   @pad: Padding character.
 */

_export
void io_format_str(struct io_output_t output, const char *str, uint16_t width, bool neg, char pad)
{
	size_t len;

	len = str_len(str);
	if(width > len) {
		uint16_t i;

		if(neg)
			io_output_write(output, str, len);

		for(i = len; i < width; i++)
			io_output_ch(output, pad);

		if(!neg)
			io_output_write(output, str, len);
	}
	else if(width > 0)
		io_output_write(output, str, width);
	else
		io_output_write(output, str, len);
}

/**
 * Format a signed integer.
 *   @output: The output device.
 *   @value: The value.
 *   @base: The base.
 *   @width: The width.
 *   @pad: Padding.
 */

_export
void io_format_int(struct io_output_t output, int value, uint8_t base, int16_t width, bool pad)
{
	if(value < 0) {
		char neg = '-';

		if(width > 0)
			width--;

		value = -value;
		io_output_write(output, &neg, 1);
	}

	io_format_uint(output, value, base, width, false, pad);
}

/**
 * Format an unsigned integer.
 *   @output: The output device.
 *   @value: The value.
 *   @base: The base.
 *   @width: The width.
 *   @neg: Negative alignment.
 *   @pad: Padding.
 */

_export
void io_format_uint(struct io_output_t output, unsigned int value, uint8_t base, int16_t width, bool neg, bool pad)
{
	uint8_t i = 0;
	uint16_t len = m_uint16max(width, 19);
	char buf[len];

	do {
		uint8_t val;

		val = value % base;
		buf[len - ++i] = val + ((val < 10) ? '0' : ('a' - 10));
	} while((value /= base) > 0);

	if(!neg) {
		while(i < width)
			buf[len - ++i] = pad ? '0' : ' ';

		io_output_write(output, buf + len - i, width ?: i);
	}
	else {
		io_output_write(output, buf + len - i, (i <= width) ? i : width);

		if(width > i) {
			uint8_t rem = width - i;

			while(i < width)
				buf[len - ++i] = ' ';

			io_output_write(output, buf + len - i, rem);
		}
	}
}

//_export
//void io_format_float(struct io_output_t output, float value, 
