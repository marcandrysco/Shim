#ifndef TYPES_VALUE_H
#define TYPES_VALUE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Value union.
 *   @integer: Integer.
 */

union value_u {
	struct value_t *base;
	struct value_integer_t *integer;
};

/**
 * Value structure.
 *   @type: The type.
 *   @data: The data union.
 */

struct value_t {
	enum type_e *type;
};


/**
 * Integer value.
 *   @base: The base value.
 *   @integer: The integer.
 */

struct value_integer_t {
	struct value_t base;

	struct integer_t *integer;
};


/*
 * value function declarations
 */

void value_delete(struct value_t *value);

void value_print(const struct value_t *value, struct io_output_t output);
struct io_chunk_t value_chunk(const struct value_t *value);

struct value_integer_t *value_integer_new(enum type_e type, struct integer_t *integer);
void value_integer_delete(struct value_integer_t *value);

void value_integer_print(const struct value_integer_t *value, struct io_output_t output);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
