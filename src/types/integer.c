#include "../common.h"
#include "integer.h"
#include "../debug/exception.h"
#include "../io/input.h"
#include "../io/output.h"
#include "../math/func.h"
#include "../mem/base.h"
#include "../mem/manage.h"
#include "../string/scan.h"
#include "../string/io.h"
#include <limits.h>


/*
 * bit manipulation definitions
 */

#define ROR(val) ((val) >> (8 * sizeof(unsigned short)))
#define ROL(val) ((val) << (8 * sizeof(unsigned short)))

#define GETLOW(val) ((val) & (unsigned int)(USHRT_MAX))
#define GETHI(val) (ROR(val) & (unsigned int)(USHRT_MAX))

#define SETLOW(val, low) (((val) & ~(unsigned int)(USHRT_MAX)) | (unsigned int)(GETLOW(low)))
#define SETHI(val, hi) (((val) & ~ROL((unsigned int)(USHRT_MAX))) | ROL((unsigned int)(GETLOW(hi))))

/*
 * local function declarations
 */

static void chunk_proc(struct io_output_t output, void *arg);

static void op_resize(struct integer_t **integer, unsigned int len);
static void op_shrink(struct integer_t **integer);
static void op_add(unsigned int *val, unsigned int add, bool *c);
static void op_sub(unsigned int *val, unsigned int sub, bool *c);
static void op_rev(unsigned int *val, unsigned int sub, bool *c);


/**
 * Create a zero integer.
 *   &returns: The zero value.
 */

_export
struct integer_t *integer_zero()
{
	struct integer_t *integer;

	integer = mem_alloc(sizeof(struct integer_t));
	integer->neg = false;
	integer->len = 0;

	return integer;
}

/**
 * Create an integer from a native value.
 *   @val: The native value.
 *   &returns: The integer.
 */

_export
struct integer_t *integer_new(int val)
{
	struct integer_t *integer;

	integer = mem_alloc(sizeof(struct integer_t) + sizeof(unsigned int));
	integer->len = 1;

	if(val >= 0) {
		integer->neg = false;
		integer->arr[0] = val;
	}
	else {
		integer->neg = true;
		integer->arr[0] = -val;
	}

	return integer;
}

/**
 * Copy an integer.
 *   @integer: The integer.
 *   &returns: The copy.
 */

_export
struct integer_t *integer_copy(const struct integer_t *integer)
{
	struct integer_t *copy;

	copy = mem_alloc(sizeof(struct integer_t) + integer->len * sizeof(unsigned int));
	copy->neg = integer->neg;
	copy->len = integer->len;
	mem_copy(copy->arr, integer->arr, integer->len * sizeof(unsigned int));

	return copy;
}

/**
 * Parse an integer from a string.
 *   @input: The input.
 *   @endptr: The end pointer.
 *   &returns: The integer.
 */

_export
struct integer_t *integer_parse(const char *str, char **endptr)
{
	int16_t ch = IO_EOS;
	struct io_input_t input;
	struct integer_t *integer;

	input = str_input_ref(&str);
	integer = integer_scan(input, &ch);
	io_input_close(input);

	if(endptr != NULL)
		*endptr = (char *)str - ((ch == IO_EOS) ? 0 : 1);

	return integer;
}

/**
 * Read an integer from the input.
 *   @input: The input.
 *   @cur: Optional. The current input.
 *   &returns: The integer.
 */

_export
struct integer_t *integer_scan(struct io_input_t input, int16_t *cur)
{
	bool neg;
	int8_t digit;
	uint8_t base;
	struct integer_t *integer;
	int16_t ch = ((cur != NULL) && (*cur != IO_EOS)) ? *cur : io_input_byte(input);

	if(ch == '-') {
		neg = true;
		ch = io_input_byte(input);
	}
	else
		neg = false;

	if(ch == '0') {
		ch = io_input_byte(input);
		if(ch == 'x') {
			base = 16;
			ch = io_input_byte(input);
		}
		else if(ch == 'b') {
			base = 2;
			ch = io_input_byte(input);
		}
		else
			base = 8;
	}
	else
		base = 10;

	digit = str_getdigit(ch);
	if(digit >= base)
		throw("Invalid number.");

	integer = integer_new((digit < 0) ? 0 : (neg ? -digit : digit));

	while(true) {
		digit = str_getdigit(ch = io_input_byte(input));
		if((digit < 0) || (digit >= base))
			break;

		integer_mul_ushort(&integer, base);
		integer_add_ushort(&integer, digit);
	}

	if(cur != NULL)
		*cur = ch;

	return integer;
}

/**
 * Delete an integer.
 *   @integer: The integer.
 */

_export
void integer_delete(struct integer_t *integer)
{
	mem_free(integer);
}


/**
 * Retrieve an unsigned 8-bit unsigned integer from the integer.
 *   @integer: The integer.
 *   &returns: The 8-bit unsigned integer.
 */

_export
uint8_t integer_uint8(struct integer_t *integer)
{
	uint8_t val;

#if UINT8_MAX <= UINT_MAX
	val = integer->arr[0];
#else
	unsigned int i;

	val = 0;
	for(i = m_uintmin(sizeof(uint8_t) / sizeof(unsigned int), integer->len) - 1; i != UINT_MAX; i--)
		val = val * ((uint8_t)1 << (8 * sizeof(unsigned int))) + integer->arr[i];
#endif

	return integer->neg ? -val : val;
}

/**
 * Check an integer to see if it fits in an unsigned 8-bit integer.
 *   @integer: The integer.
 *   &returns: The range error result.
 */

_export
int8_t integer_uint8chk(struct integer_t *integer)
{
	if(integer->neg)
		return -1;
#if UINT8_MAX < UINT_MAX
	else if((integer->len > 1) || (integer->arr[0] > UINT8_MAX))
#else
	else if(integer->len > (sizeof(uint8_t) / sizeof(unsigned int)))
#endif
		return 0;
	else
		return 1;
}

/**
 * Retrieve an unsigned 16-bit unsigned integer from the integer.
 *   @integer: The integer.
 *   &returns: The 16-bit unsigned integer.
 */

_export
uint16_t integer_uint16(struct integer_t *integer)
{
	uint16_t val;

#if UINT16_MAX <= UINT_MAX
	val = integer->arr[0];
#else
	unsigned int i;

	val = 0;
	for(i = m_uintmin(sizeof(uint16_t) / sizeof(unsigned int), integer->len) - 1; i != UINT_MAX; i--)
		val = val * ((uint16_t)1 << (8 * sizeof(unsigned int))) + integer->arr[i];
#endif

	return integer->neg ? -val : val;
}

/**
 * Check an integer to see if it fits in an unsigned 16-bit integer.
 *   @integer: The integer.
 *   &returns: The range error result.
 */

_export
int8_t integer_uint16chk(struct integer_t *integer)
{
	if(integer->neg)
		return -1;
#if UINT16_MAX < UINT_MAX
	else if((integer->len > 1) || (integer->arr[0] > UINT16_MAX))
#else
	else if(integer->len > (sizeof(uint16_t) / sizeof(unsigned int)))
#endif
		return 0;
	else
		return 1;
}

/**
 * Retrieve an unsigned 32-bit unsigned integer from the integer.
 *   @integer: The integer.
 *   &returns: The 32-bit unsigned integer.
 */

_export
uint32_t integer_uint32(struct integer_t *integer)
{
	uint32_t val;

#if UINT32_MAX <= UINT_MAX
	val = integer->arr[0];
#else
	unsigned int i;

	val = 0;
	for(i = m_uintmin(sizeof(uint32_t) / sizeof(unsigned int), integer->len) - 1; i != UINT_MAX; i--)
		val = val * ((uint32_t)1 << (8 * sizeof(unsigned int))) + integer->arr[i];
#endif

	return integer->neg ? -val : val;
}

/**
 * Check an integer to see if it fits in an unsigned 32-bit integer.
 *   @integer: The integer.
 *   &returns: The range error result.
 */

_export
int8_t integer_uint32chk(struct integer_t *integer)
{
	if(integer->neg)
		return -1;
#if UINT32_MAX < UINT_MAX
	else if((integer->len > 1) || (integer->arr[0] > UINT32_MAX))
#else
	else if(integer->len > (sizeof(uint32_t) / sizeof(unsigned int)))
#endif
		return 0;
	else
		return 1;
}

/**
 * Retrieve an unsigned 64-bit unsigned integer from the integer.
 *   @integer: The integer.
 *   &returns: The 64-bit unsigned integer.
 */

_export
uint64_t integer_uint64(struct integer_t *integer)
{
	uint64_t val;

#if UINT64_MAX <= UINT_MAX
	val = integer->arr[0];
#else
	unsigned int i;

	val = 0;
	for(i = m_uintmin(sizeof(uint64_t) / sizeof(unsigned int), integer->len) - 1; i != UINT_MAX; i--)
		val = val * ((uint64_t)1 << (8 * sizeof(unsigned int))) + integer->arr[i];
#endif

	return integer->neg ? -val : val;
}

/**
 * Check an integer to see if it fits in an unsigned 64-bit integer.
 *   @integer: The integer.
 *   &returns: The range error result.
 */

_export
int8_t integer_uint64chk(struct integer_t *integer)
{
	if(integer->neg)
		return -1;
#if UINT64_MAX < UINT_MAX
	else if((integer->len > 1) || (integer->arr[0] > UINT64_MAX))
#else
	else if(integer->len > (sizeof(uint64_t) / sizeof(unsigned int)))
#endif
		return 0;
	else
		return 1;
}


/**
 * Add an integer to a base integer.
 *   @integer: The base integer.
 *   @val: The value integer to add.
 */

_export
void integer_add(struct integer_t **integer, struct integer_t *val)
{
	if(val->neg) {
		val->neg = false;
		integer_sub(integer, val);
		val->neg = true;
	}
	else {
		bool c;
		unsigned int i;

		if(val->len > (*integer)->len)
			op_resize(integer, val->len);

		for(i = 0, c = false; (i < val->len) || (c && (i < (*integer)->len)); i++)
			op_add(&(*integer)->arr[i], (i < val->len) ? val->arr[i] : 0, &c);

		if(c) {
			*integer = mem_realloc(*integer, sizeof(struct integer_t) + (i + 1) * sizeof(unsigned int));
			(*integer)->len = i + 1;
			(*integer)->arr[i] = 1;
		}
	}
}

/**
 * Add an unsigned integer to the integer.
 *   @integer: The integer.
 *   @val: The value to add.
 */

_export
void integer_add_uint(struct integer_t **integer, unsigned int val)
{
	unsigned int i, t;

	for(i = 0; i < (*integer)->len; i++) {
		t = (*integer)->arr[i];
		(*integer)->arr[i] += val;

		if((*integer)->arr[i] >= t)
			return;

		val = 1;
	}

	if(val > 0) {
		*integer = mem_realloc(*integer, sizeof(struct integer_t) + (i + 1) * sizeof(unsigned int));
		(*integer)->len = i + 1;
		(*integer)->arr[i] = val;
	}
}

/**
 * Add an unsigned short to the integer.
 *   @integer: The integer.
 *   @val: The value to add.
 */

_export
void integer_add_ushort(struct integer_t **integer, unsigned short val)
{
	integer_add_uint(integer, val);
}


/**
 * Subtract an integer to a base integer.
 *   @integer: The base integer.
 *   @val: The value integer to subtract.
 */

_export
void integer_sub(struct integer_t **integer, struct integer_t *val)
{
	if(val->neg) {
		val->neg = false;
		integer_add(integer, val);
		val->neg = true;
	}
	else if(integer_cmp(*integer, val) >= 0) {
		bool c;
		unsigned int i;

		for(i = 0, c = false; (i < val->len) || (c && (i < (*integer)->len)); i++)
			op_sub(&(*integer)->arr[i], (i < val->len) ? val->arr[i] : 0, &c);

		op_shrink(integer);
	}
	else {
		bool c;
		unsigned int i;

		op_resize(integer, val->len);

		for(i = 0, c = false; i < (*integer)->len; i++)
			op_rev(&(*integer)->arr[i], (i < val->len) ? val->arr[i] : 0, &c);

		(*integer)->neg = true;
		op_shrink(integer);
	}
}


/**
 * Multiply the integer by an unsigned short.
 *   @integer: The integer.
 *   @val: The value to multiply.
 */

_export
void integer_mul_ushort(struct integer_t **integer, unsigned short val)
{
	unsigned int i, mul, carry = 0;

	for(i = 0; i < (*integer)->len; i++) {
		mul = GETLOW((*integer)->arr[i]) * val + carry;
		(*integer)->arr[i] = SETLOW((*integer)->arr[i], mul);
		carry = GETHI(mul);

		mul = GETHI((*integer)->arr[i]) * val + carry;
		(*integer)->arr[i] = SETHI((*integer)->arr[i], mul);
		carry = GETHI(mul);
	}

	if(carry > 0) {
		*integer = mem_realloc(*integer, sizeof(struct integer_t) + (i + 1) * sizeof(unsigned int));
		(*integer)->len = i + 1;
		(*integer)->arr[i] = carry;
	}
}


/**
 * Divide the integer by an unsigned short.
 *   @integer: The integer.
 *   @val: The value to divide.
 *   &returns: The remainder
 */

_export
unsigned short integer_div_ushort(struct integer_t **integer, unsigned short val)
{
	unsigned int i, rem = 0;

	for(i = (*integer)->len - 1; i != UINT_MAX; i--) {
		rem = ROL(rem) + GETHI((*integer)->arr[i]);
		(*integer)->arr[i] = SETHI((*integer)->arr[i], rem / val);
		rem %= val;

		rem = ROL(rem) + GETLOW((*integer)->arr[i]);
		(*integer)->arr[i] = SETLOW((*integer)->arr[i], rem / val);
		rem %= val;
	}

	for(i = (*integer)->len - 1; i != UINT_MAX; i--) {
		if((*integer)->arr[i] > 0)
			break;
	}

	if(++i != (*integer)->len) {
		*integer = mem_realloc(*integer, sizeof(struct integer_t) + i * sizeof(unsigned int));
		(*integer)->len = i;
	}

	return rem;
}


/**
 * Compare two integer.
 *   @left: The left integer.
 *   @right: The right integer.
 *   &returns: Their order.
 */

int integer_cmp(const struct integer_t *left, const struct integer_t *right)
{
	unsigned int i;

	if(left->len > right->len)
		return 2;
	else if(left->len < right->len)
		return -2;

	for(i = left->len - 1; i != UINT_MAX; i--) {
		if(left->arr[i] > right->arr[i])
			return 1;
		else if(left->arr[i] < right->arr[i])
			return -1;
	}

	return 0;
}


/**
 * Print an integer to the output.
 *   @integer: The integer.
 *   @output: The output.
 */

_export
void integer_print(const struct integer_t *integer, struct io_output_t output)
{
	struct integer_t *copy;
	char *str, buf[3 * integer->len * sizeof(unsigned int) + 1];

	str = buf + sizeof(buf) - 1;

	copy = integer_copy(integer);

	do
		*str-- = '0' + integer_div_ushort(&copy, 10);
	while(!integer_iszero(copy));

	integer_delete(copy);

	if(integer->neg)
		*str-- = '-';

	io_output_write(output, str+1, sizeof(buf) - (str - buf + 1) );
}

/**
 * Retrieve an I/O chunk for the integer.
 *   @integer: The integer.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t integer_chunk(const struct integer_t *integer)
{
	return (struct io_chunk_t){ chunk_proc, (void *)integer };
}

/**
 * Process a chunk callback.
 *   @output: The output.
 *   @arg: The argument.
 */

static void chunk_proc(struct io_output_t output, void *arg)
{
	integer_print(arg, output);
}



/**
 * Resize an integer to a given length, zero filling all new elements.
 *   @integer: The integer.
 *   @len: The length.
 */

static void op_resize(struct integer_t **integer, unsigned int len)
{
	unsigned int i;

	*integer = mem_realloc(*integer, sizeof(struct integer_t) + len * sizeof(unsigned int));

	for(i = (*integer)->len; i < len; i++)
		(*integer)->arr[i] = 0;

	(*integer)->len = len;
}

/**
 * Shrink the integer to the minimum necessary size.
 *   @integer: The integer.
 */

static void op_shrink(struct integer_t **integer)
{
	unsigned int i;

	for(i = (*integer)->len; i > 0; i--) {
		if((*integer)->arr[i-1] != 0)
			break;
	}

	if(i < (*integer)->len) {
		*integer = mem_realloc(*integer, sizeof(struct integer_t) + i * sizeof(unsigned int));
		(*integer)->len = i;
	}
}

/**
 * Perform an add with carry on a single digit.
 *   @val: The base value.
 *   @add: The value to add.
 *   @c: The carry flag.
 */

static void op_add(unsigned int *val, unsigned int add, bool *c)
{
	unsigned int t;

	t = *val;
	*val += add + (*c ? 1 : 0);
	*c = (*val < t) || ((*val == t) && *c);
}

/**
 * Perform a subtract with carry on a single digit.
 *   @val: The base value.
 *   @sub: The value to subract.
 *   @c: The carry flag.
 */

static void op_sub(unsigned int *val, unsigned int sub, bool *c)
{
	unsigned int t;

	t = *val;
	*val -= sub + (*c ? 1 : 0);
	*c = (*val > t) || ((*val == t) && *c);
}

/**
 * Perform a reverse subtract with carry on a single digit.
 *   @val: The base value.
 *   @sub: The value to subract from.
 *   @c: The carry flag.
 */

static void op_rev(unsigned int *val, unsigned int sub, bool *c)
{
	unsigned int t;

	t = sub;
	*val = sub - (*val + (*c ? 1 : 0));
	*c = (*val > t) || ((*val == t) && *c);
}
