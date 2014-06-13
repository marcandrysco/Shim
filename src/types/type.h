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
 *   @type_int_e: Integer.
 *   @type_uint_e: Unsigned integer.
 *   @type_int32_t: 32-bit integer.
 */

enum type_e {
	type_int_e,
	type_uint_e,
	type_int32_e
};

/**
 * Type union.
 *   @id: The identifier.
 */

union type_u {
	enum type_e *id;
};


/*
 * type function declarations
 */

enum type_e *type_copy(const enum type_e *type);
void type_delete(enum type_e *type);

enum type_e *type_native_new(enum type_e id);
void type_native_delete(enum type_e *type);

bool type_isnative(enum type_e id);
const char *type_name(enum type_e id);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
