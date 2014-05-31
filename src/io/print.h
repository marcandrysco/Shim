#ifndef IO_PRINT_H
#define IO_PRINT_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_chunk_t;


/**
 * Print callback modifiers structure.
 *   @zero, neg: Zero padding, and negative flag.
 *   @width: The field width.
 */

struct io_print_mod_t {
	bool zero, neg;
	uint16_t width;
};

/**
 * Printing callback funtion.
 *   @output: The output device.
 *   @mod: Modifiers.
 *   @args: The variable argument list. Read only the parameter expected.
 */

typedef void (*io_print_f)(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);

/**
 * Printing callback structure.
 *   @ch: The short character. Set to '\0' if unused.
 *   @name: The long name. Set to 'NULL' if unused.
 *   @callback: The callback function.
 */

struct io_print_t {
	char ch;
	const char *name;
	uint16_t flags;
	io_print_f callback;
};


/*
 * printing function declarations
 */

void io_print_str(struct io_output_t output, const char *str);
void io_print_int(struct io_output_t output, int value);
void io_print_uint(struct io_output_t output, unsigned int value);

void io_printf(struct io_output_t output, const char *restrict format, ...);
void io_vprintf(struct io_output_t output, const char *restrict format, va_list args);
void io_vprintf_custom(struct io_output_t output, struct io_print_t *print, const char *format, struct arglist_t *args);

void io_printf_chunk(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_str(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_int(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_uint(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_hex(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_char(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_float(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);

void io_format_chunk(struct io_output_t output, struct io_chunk_t chunk, uint16_t width, bool neg, char pad);
void io_format_str(struct io_output_t output, const char *str, uint16_t width, bool neg, char pad);
void io_format_int(struct io_output_t output, int value, uint8_t base, int16_t width, bool pad);
void io_format_uint(struct io_output_t output, unsigned int value, uint8_t base, int16_t width, bool pad);

/*
 * convenience defintions
 */

#define printf(...) io_printf(io_stdout, __VA_ARGS__)

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
