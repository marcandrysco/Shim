#ifndef IO_OBJECT_H
#define IO_OBJECT_H

/*
 * definitions
 */

#include "../types/defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_output_t;


/**
 * I/O object processing callback.
 *   @ref: The reference.
 *   @output: The output.
 */

typedef void (*io_object_f)(void *ref, struct io_output_t output);

/**
 * I/O object interface.
 *   @proc: Processing function.
 *   @delete: Deletion.
 */

struct io_object_i {
	io_object_f proc;
	delete_f delete;
};

/**
 * Object structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_object_t {
	void *ref;
	const struct io_object_i *iface;
};


/*
 * i/o object variables
 */

extern struct io_object_t io_object_null;

/*
 * i/o object function declarations
 */

struct io_object_t io_object_new(void *ref, io_object_f proc, delete_f delete);
void io_object_proc(struct io_object_t object, struct io_output_t output);
void io_object_delete(struct io_object_t object);

struct io_object_t io_object_copy(const char *path);


/**
 * Test if an I/O object is null.
 *   @object: The object.
 *   &returns: True if null, false otherwise.
 */

static inline bool io_object_isnull(struct io_object_t object)
{
	return (object.ref == NULL) && (object.iface == NULL);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
