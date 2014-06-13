#include "../common.h"
#include "type.h"
#include "../debug/exception.h"
#include "../mem/manage.h"


/**
 * Copy a type.
 *   @type: The type.
 *   &returns: The copy.
 */

_export
enum type_e *type_copy(const enum type_e *type)
{
	if(type_isnative(*type))
		return type_native_new(*type);
	else
		_fatal("stub");
}

/**
 * Deelte a type.
 *   @type: The type.
 */

_export
void type_delete(enum type_e *type)
{
	if(type_isnative(*type))
		type_native_delete(type);
	else
		_fatal("stub");
}


/**
 * Create a new native type.
 *   @id: The identifier.
 *   &returns: The type.
 */

_export
enum type_e *type_native_new(enum type_e id)
{
	enum type_e *type;

	type = mem_alloc(sizeof(enum type_e));
	*type = id;

	return type;
}

/**
 * Delete the native type.
 *   @type: The type.
 */

_export
void type_native_delete(enum type_e *type)
{
	mem_free(type);
}


/**
 * Determine if a type identifier is native.
 *   @id: The identifier.
 *   &returns: True if native, false otherwise.
 */

_export
bool type_isnative(enum type_e id)
{
	switch(id) {
	case type_int_e:
	case type_uint_e:
	case type_int32_e:
		return true;
	}

	throw("Invalid type identifier.");
}

/**
 * Retrieve the name for the type identifier.
 *   @id: The identifier.
 *   &returns: The string name.
 */

_export
const char *type_name(enum type_e id)
{
	switch(id) {
	case type_int_e: return "int"; break;
	case type_uint_e: return "uint"; break;
	case type_int32_e: return "int32"; break;
	}

	throw("Invalid type identifier.");
}
