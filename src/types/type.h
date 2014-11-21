#ifndef TYPES_TYPE_H
#define TYPES_TYPE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Type enumerator.
 *   @type_void_e: Void.
 *   @type_int_e: Integer.
 *   @type_uint_e: Unsigned integer.
 *   @type_int32_e: 32-bit integer.
 *   @type_uint32_e: 32-bit unsigned integer.
 *   @type_ptr_e: Pointer.
 *   @type_func_e: Function.
 */

enum type_e {
	type_void_e,
	type_int_e,
	type_uint_e,
	type_int32_e,
	type_uint32_e,
	type_ptr_e,
	type_func_e,
};

/**
 * Type union.
 *   @id: The identifier.
 *   @ptr: The pointer.
 *   @func: The function.
 */

union type_u {
	enum type_e *id;
	struct type_ptr_t *ptr;
	struct type_func_t *func;
};


/**
 * Pointer type structure.
 *   @id: The type identifier.
 *   @subtype: The sub-type.
 */

struct type_ptr_t {
	enum type_e id;

	enum type_e *subtype;
};

/**
 * Function type structure.
 *   @id: The type identifier.
 *   @ret, param: The return and array of parameters.
 *   @nparam: The number of parameters.
 */

struct type_func_t {
	enum type_e id;

	enum type_e *ret, **param;
	unsigned int nparam;
};


/*
 * type function declarations
 */

enum type_e *type_copy(const enum type_e *type);
void type_delete(enum type_e *type);

bool type_isequal(enum type_e *left, enum type_e *right);

void type_print(const enum type_e *type, struct io_output_t output);
struct io_chunk_t type_chunk(const enum type_e *type);

/*
 * void type function declarations
 */

enum type_e *type_void_new();
void type_void_delete(enum type_e *type);

bool type_void_isequal(enum type_e *left, enum type_e *right);

void type_void_print(const enum type_e *type, struct io_output_t output);
struct io_chunk_t type_void_chunk(const enum type_e *type);

/*
 * native type function declarations
 */

enum type_e *type_native_new(enum type_e id);
void type_native_delete(enum type_e *type);

bool type_native_isequal(enum type_e *left, enum type_e *right);

void type_native_print(const enum type_e *type, struct io_output_t output);
struct io_chunk_t type_native_chunk(const enum type_e *type);

/*
 * pointer type function declarations
 */

struct type_ptr_t *type_ptr_new(enum type_e *subtype);
struct type_ptr_t *type_ptr_copy(const struct type_ptr_t *type);
void type_ptr_delete(struct type_ptr_t *type);

/*
 * function type function declarations
 */

struct type_func_t *type_func_new(enum type_e *ret, enum type_e **param, unsigned int nparam);
struct type_func_t *type_func_copy(const struct type_func_t *type);
void type_func_delete(struct type_func_t *type);

bool type_func_isequal(struct type_func_t *left, struct type_func_t *right);

void type_func_print(const struct type_func_t *type, struct io_output_t output);
struct io_chunk_t type_func_chunk(const struct type_func_t *type);

/*
 * type helper functions
 */

bool type_isnative(enum type_e id);
bool type_isint(enum type_e id);
bool type_issigned(enum type_e id);
const char *type_name(enum type_e id);


/**
 * Replace a destination type with the source type.
 *   @dest: The destination type pointer.
 *   @src; The source type.
 */

static inline void type_replace(enum type_e **dest, enum type_e *src)
{
	if(*dest != NULL)
		type_delete(*dest);

	*dest = src;
}


/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
