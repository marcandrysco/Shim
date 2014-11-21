#include "../common.h"
#include "type.h"
#include "../debug/exception.h"
#include "../io/print.h"
#include "../mem/manage.h"


/*
 * local function declaratoins
 */

static void proc_type(struct io_output_t output, void *arg);

static void proc_void(struct io_output_t output, void *arg);
static void proc_native(struct io_output_t output, void *arg);
static void proc_func(struct io_output_t output, void *arg);


/**
 * Copy a type.
 *   @type: The type.
 *   &returns: The copy.
 */

_export
enum type_e *type_copy(const enum type_e *type)
{
	union type_u utype = { .id = (enum type_e *)type };

	if(*type == type_void_e)
		return type_void_new();
	else if(type_isnative(*type))
		return type_native_new(*type);
	else if(*type == type_func_e)
		return &type_func_copy(utype.func)->id;
	else if(*type == type_ptr_e)
		return &type_ptr_copy(utype.ptr)->id;
	else
		_fatal("Invalid type");
}

/**
 * Deelte a type.
 *   @type: The type.
 */

_export
void type_delete(enum type_e *type)
{
	union type_u utype = { .id = type };

	if(*type == type_void_e)
		type_void_delete(type);
	else if(type_isnative(*type))
		type_native_delete(type);
	else if(*type == type_func_e)
		type_func_delete(utype.func);
	else
		_fatal("Invalid type");
}


/**
 * Check if two types are equal.
 *   @left: The left type.
 *   @right: The right type.
 *   &returns: True if equal, false otherwise.
 */

_export
bool type_isequal(enum type_e *left, enum type_e *right)
{
	union type_u uleft = { .id = left }, uright = { .id = right };

	if((*left == type_void_e) && (*right == type_void_e))
		return type_void_isequal(left, right);
	else if(type_isnative(*left) && type_isnative(*right))
		return type_native_isequal(left, right);
	else if((*left == type_func_e) && (*right == type_func_e))
		return type_func_isequal(uleft.func, uright.func);
	else
		return false;
}


/**
 * Print a type.
 *   @type: The type.
 *   @output: The output.
 */

_export
void type_print(const enum type_e *type, struct io_output_t output)
{
	union type_u utype = { .id = (enum type_e *)type };

	if(*type == type_void_e)
		type_void_print(type, output);
	else if(type_isnative(*type))
		type_native_print(type, output);
	else if(*type == type_func_e)
		type_func_print(utype.func, output);
	//else if(*type == type_ptr_e)
		//return &type_ptr_copy(utype.ptr)->id;
	else
		_fatal("Invalid type");
}

/**
 * Create a chunk for a type.
 *   @type: The type.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t type_chunk(const enum type_e *type)
{
	return (struct io_chunk_t){ proc_type, (void *)type };
}

/**
 * Process a type chunk.
 *   @output: The output.
 *   @arg: The argument.
 */

static void proc_type(struct io_output_t output, void *arg)
{
	type_print(arg, output);
}


/**
 * Create a new void type.
 *   @id: The identifier.
 *   &returns: The type.
 */

_export
enum type_e *type_void_new(enum type_e id)
{
	enum type_e *type;

	type = mem_alloc(sizeof(enum type_e));
	*type = type_void_e;

	return type;
}

/**
 * Delete the void type.
 *   @type: The type.
 */

_export
void type_void_delete(enum type_e *type)
{
	mem_free(type);
}


/**
 * Check if two void types are equal.
 *   @left: The left type.
 *   @right: The right type.
 *   &returns: True if equal, false otherwise.
 */

_export
bool type_void_isequal(enum type_e *left, enum type_e *right)
{
	return true;
}


/**
 * Print a void type.
 *   @type: The type.
 *   @output: The output.
 */

_export
void type_void_print(const enum type_e *type, struct io_output_t output)
{
	io_printf(output, "%s", type_name(*type));
}

/**
 * Create a chunk for a void type.
 *   @type: The type.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t type_void_chunk(const enum type_e *type)
{
	return (struct io_chunk_t){ proc_void, (void *)type };
}

/**
 * Process a void type chunk.
 *   @output: The output.
 *   @arg: The argument.
 */

static void proc_void(struct io_output_t output, void *arg)
{
	type_native_print(arg, output);
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
 * Check if two native types are equal.
 *   @left: The left type.
 *   @right: The right type.
 *   &returns: True if equal, false otherwise.
 */

_export
bool type_native_isequal(enum type_e *left, enum type_e *right)
{
	return *left == *right;
}


/**
 * Print a native type.
 *   @type: The type.
 *   @output: The output.
 */

_export
void type_native_print(const enum type_e *type, struct io_output_t output)
{
	io_printf(output, "%s", type_name(*type));
}

/**
 * Create a chunk for a native type.
 *   @type: The type.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t type_native_chunk(const enum type_e *type)
{
	return (struct io_chunk_t){ proc_native, (void *)type };
}

/**
 * Process a native type chunk.
 *   @output: The output.
 *   @arg: The argument.
 */

static void proc_native(struct io_output_t output, void *arg)
{
	type_native_print(arg, output);
}


/**
 * Create a new pointer type.
 *   @subtype: The subtype.
 *   &returns: The pointer type.
 */

_export
struct type_ptr_t *type_ptr_new(enum type_e *subtype)
{
	struct type_ptr_t *type;

	type = mem_alloc(sizeof(struct type_ptr_t));
	type->id = type_ptr_e;
	type->subtype = subtype;

	return type;
}

/**
 * Copy a pointer type.
 *   @type: The pointer type.
 *   &returns: The copy.
 */

_export
struct type_ptr_t *type_ptr_copy(const struct type_ptr_t *type)
{
	struct type_ptr_t *copy;

	copy = mem_alloc(sizeof(struct type_ptr_t));
	copy->id = type_ptr_e;
	copy->subtype = type_copy(type->subtype);

	return copy;
}

/**
 * Delete a pointer type.
 *   @type: The pointer type.
 */

_export
void type_ptr_delete(struct type_ptr_t *type)
{
	type_delete(type->subtype);
	mem_free(type);
}


/**
 * Create a new function type.
 *   @ret: The return type.
 *   @param: The array of parameters.
 *   @nparam: The number of parameters.
 *   &returns: The function type.
 */

_export
struct type_func_t *type_func_new(enum type_e *ret, enum type_e **param, unsigned int nparam)
{
	unsigned int i;
	struct type_func_t *type;

	type = mem_alloc(sizeof(struct type_func_t));
	type->id = type_func_e;
	type->ret = ret;
	type->nparam = nparam;
	type->param = mem_alloc(nparam * sizeof(enum type_e *));

	for(i = 0; i < nparam; i++)
		type->param[i] = param[i];

	return type;
}

/**
 * Copy a function type.
 *   @type: The function type.
 *   &return: The copy.
 */

_export
struct type_func_t *type_func_copy(const struct type_func_t *type)
{
	unsigned int i;
	struct type_func_t *copy;

	copy = mem_alloc(sizeof(struct type_func_t));
	copy->id = type_func_e;
	copy->ret = type_copy(type->ret);
	copy->nparam = type->nparam;
	copy->param = mem_alloc(type->nparam * sizeof(enum type_e *));

	for(i = 0; i < type->nparam; i++)
		copy->param[i] = type_copy(type->param[i]);

	return copy;
}

/**
 * Delete a function type.
 *   @type: The function type.
 */

_export
void type_func_delete(struct type_func_t *type)
{
	unsigned int i;

	for(i = 0; i < type->nparam; i++)
		type_delete(type->param[i]);

	type_delete(type->ret);
	mem_free(type->param);
	mem_free(type);
}


/**
 * Check if two function types are equal.
 *   @left: The left type.
 *   @right: The right type.
 *   &returns: True if equal, false otherwise.
 */

_export
bool type_func_isequal(struct type_func_t *left, struct type_func_t *right)
{
	unsigned int i;

	if(!type_isequal(left->ret, right->ret))
		return false;

	if(left->nparam != right->nparam)
		return false;

	for(i = 0; i < left->nparam; i++) {
		if(!type_isequal(left->param[i], right->param[i]))
			return false;
	}

	return true;
}


/**
 * Print a function type.
 *   @type: The type.
 *   @output: The output.
 */

_export
void type_func_print(const struct type_func_t *type, struct io_output_t output)
{
	io_printf(output, "%C()", type_chunk(type->ret));
}

/**
 * Create a chunk for a function type.
 *   @type: The type.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t type_func_chunk(const struct type_func_t *type)
{
	return (struct io_chunk_t){ proc_func, (void *)type };
}

/**
 * Process a function type chunk.
 *   @output: The output.
 *   @arg: The argument.
 */

static void proc_func(struct io_output_t output, void *arg)
{
	type_func_print(arg, output);
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
	case type_uint32_e:
		return true;

	case type_void_e:
	case type_ptr_e:
	case type_func_e:
		return false;
	}

	throw("Invalid type identifier.");
}

/**
 * Determine if a type identifier is an integer.
 *   @id: The identifier.
 *   &returns: True if integer, false otherwise.
 */

_export
bool type_isint(enum type_e id)
{
	switch(id) {
	case type_int_e:
	case type_uint_e:
	case type_int32_e:
	case type_uint32_e:
		return true;

	case type_void_e:
	case type_ptr_e:
	case type_func_e:
		return false;
	}

	throw("Invalid type identifier.");
}
/**
 * Determine if a type identifier is signed value.
 *   @id: The identifier.
 *   &returns: True if signed, false otherwise.
 */

_export
bool type_issigned(enum type_e id)
{
	switch(id) {
	case type_int_e:
	case type_int32_e:
		return true;

	case type_uint_e:
	case type_uint32_e:
	case type_void_e:
	case type_ptr_e:
	case type_func_e:
		return false;
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
	case type_void_e: return "void"; break;
	case type_int_e: return "int"; break;
	case type_uint_e: return "uint"; break;
	case type_int32_e: return "int32"; break;
	case type_uint32_e: return "uint32"; break;
	case type_ptr_e: return "ptr"; break;
	case type_func_e: return "func"; break;
	}

	throw("Invalid type identifier.");
}
