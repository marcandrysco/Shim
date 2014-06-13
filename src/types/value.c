#include "../common.h"
#include "value.h"
#include "../debug/exception.h"
#include "../mem/manage.h"
#include "../io/print.h"
#include "integer.h"
#include "type.h"


/*
 * local function declarations
 */

static void chunk_proc(struct io_output_t output, const struct value_t *value);


/**
 * Delete a value.
 *   @value: The value.
 */

_export
void value_delete(struct value_t *value)
{
	union value_u uvalue = { .base = value };

	if(type_isnative(*value->type))
		value_integer_delete(uvalue.integer);
	else
		_fatal("Invalid value.");
}


/**
 * Print an value to the output.
 *   @value: The value.
 *   @output: The output.
 */

_export
void value_print(const struct value_t *value, struct io_output_t output)
{
	union value_u uvalue = { .base = (struct value_t *)value };

	if(type_isnative(*value->type))
		value_integer_print(uvalue.integer, output);
	else
		_fatal("Invalid value.");
}

/**
 * Create a chunk for the value.
 *   @value: The value.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t value_chunk(const struct value_t *value)
{
	return (struct io_chunk_t){ (io_chunk_f)chunk_proc, (void *)value };
}

/**
 * Process a chunk for a value.
 *   @output: The output.
 *   @value: The value.
 */

static void chunk_proc(struct io_output_t output, const struct value_t *value)
{
	value_print(value, output);
}


/**
 * Create an integer value.
 *   @type: The type. Must be a native type.
 *   @integer: Consumed. The integer.
 *   &returns: The value.
 */

_export
struct value_integer_t *value_integer_new(enum type_e type, struct integer_t *integer)
{
	struct value_integer_t *value;

	if(!type_isnative(type))
		throw("Cannot create an integer from a non-native type.");

	value = mem_alloc(sizeof(struct value_integer_t));
	value->base.type = type_native_new(type);
	value->integer = integer;

	return value;
}

/**
 * Delete an integer value.
 *   @value: The value.
 */

_export
void value_integer_delete(struct value_integer_t *value)
{
	integer_delete(value->integer);
	type_delete(value->base.type);
	mem_free(value);
}


/**
 * Print an integer value to the output.
 *   @value: The value.
 *   @output: The output.
 */

_export
void value_integer_print(const struct value_integer_t *value, struct io_output_t output)
{
	io_printf(output, "int <%C>", integer_chunk(value->integer));
}
