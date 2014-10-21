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
static void chunk_proc_raw(struct io_output_t output, const struct value_t *value);


/**
 * Copy a value.
 *   @value: The original value.
 *   &returns: The copy.
 */

_export
struct value_t *value_copy(struct value_t *value)
{
	union value_u uvalue = { .base = value };

	if(type_isnative(*value->type))
		return &value_integer_copy(uvalue.integer)->base;
	else
		_fatal("Invalid value.");
}

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
 * Print a raw value to the output.
 *   @value: The value.
 *   @output: The output.
 */

_export
void value_print_raw(const struct value_t *value, struct io_output_t output)
{
	union value_u uvalue = { .base = (struct value_t *)value };

	if(type_isnative(*value->type))
		value_integer_print_raw(uvalue.integer, output);
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
 * Create a raw chunk for the value.
 *   @value: The value.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t value_chunk_raw(const struct value_t *value)
{
	return (struct io_chunk_t){ (io_chunk_f)chunk_proc_raw, (void *)value };
}

/**
 * Process a chunk for a value.
 *   @output: The output.
 *   @value: The value.
 */

static void chunk_proc_raw(struct io_output_t output, const struct value_t *value)
{
	value_print_raw(value, output);
}


/**
 * Create an integer value.
 *   @type: The type. Must be an integer type.
 *   @integer: Consumed. The integer.
 *   &returns: The value.
 */

_export
struct value_integer_t *value_integer_new(enum type_e type, struct integer_t *integer)
{
	struct value_integer_t *value;

	if(!type_isint(type))
		throw("Cannot create an integer from a non-native type.");

	value = mem_alloc(sizeof(struct value_integer_t));
	value->base.type = type_native_new(type);
	value->integer = integer;

	return value;
}

/**
 * Copy an integer value.
 *   @value: The original value.
 *   &returns: The copy.
 */

_export
struct value_integer_t *value_integer_copy(struct value_integer_t *value)
{
	struct value_integer_t *copy;

	if(!type_isnative(*value->base.type))
		throw("Cannot create an integer from a non-native type.");

	copy = mem_alloc(sizeof(struct value_integer_t));
	copy->base.type = type_copy(value->base.type);
	copy->integer = integer_copy(value->integer);

	return copy;
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
	io_printf(output, "%C <%C>", type_chunk(value->base.type), integer_chunk(value->integer));
}

/**
 * Print an integer value to the output.
 *   @value: The value.
 *   @output: The output.
 */

_export
void value_integer_print_raw(const struct value_integer_t *value, struct io_output_t output)
{
	io_printf(output, "%C", integer_chunk(value->integer));
}
