#ifndef TYPE_INTEGER_H
#define TYPE_INTEGER_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Integer structure.
 *   @neg: Negative flag.
 *   @len: The length.
 *   @arr: The array.
 */

struct integer_t {
	bool neg;
	unsigned int len;

	unsigned int arr[];
};


/*
 * integer function declarations
 */

struct integer_t *integer_zero();
struct integer_t *integer_new(int val);
struct integer_t *integer_copy(const struct integer_t *integer);
struct integer_t *integer_parse(const char *str, char **endptr);
struct integer_t *integer_scan(struct io_input_t input, int16_t *cur);
void integer_delete(struct integer_t *integer);

void integer_add_uint(struct integer_t **integer, unsigned int val);
void integer_add_ushort(struct integer_t **integer, unsigned short val);

void integer_mul_ushort(struct integer_t **integer, unsigned short val);

unsigned short integer_div_ushort(struct integer_t **integer, unsigned short val);

void integer_print(const struct integer_t *integer, struct io_output_t output);
struct io_chunk_t integer_chunk(const struct integer_t *integer);


/**
 * Test if an integer is zero.
 *   @integer: The itneger.
 *   &returns: True if zero, false otherwise.
 */

static inline bool integer_iszero(const struct integer_t *integer)
{
	return integer->len == 0;
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
