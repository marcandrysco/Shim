#include "../common.h"
#include "object.h"
#include "../mem/manage.h"
#include "../string/base.h"
#include "input.h"
#include "output.h"


/**
 * Instance structure.
 *   @ref: The reference.
 *   @proc: The processing callback.
 *   @delete: The delete.
 */

struct inst_t {
	void *ref;

	io_object_f proc;
	delete_f delete;
};


/**
 * Segment structure.
 *   @file: The file.
 *   @offset: The offset.
 *   @nbytes: The number of bytes.
 */

struct segment_t {
	struct io_file_t file;
	uint64_t offset, nbytes;
};


/*
 * local function declarations
 */

static void inst_proc(struct inst_t *inst, struct io_output_t output);
static void inst_delete(struct inst_t *inst);

static void copy_proc(char *path, struct io_output_t output);

static void segment_proc(struct segment_t *segment, struct io_output_t output);;

/*
 * local variables
 */

static struct io_object_i inst_iface = { (io_object_f)inst_proc, (delete_f)inst_delete };
static struct io_object_i copy_iface = { (io_object_f)copy_proc, mem_free };
static struct io_object_i segment_iface = { (io_object_f)segment_proc, mem_free };

/*
 * global variables
 */

_export struct io_object_t io_object_null = { NULL, NULL };


/**
 * Create an I/O object with static processor and delete callbacks.
 *   @ref: The reference.
 *   @proc: The processing callback.
 *   @delete: The delete callback.
 *   &returns: The I/O object.
 */

_export
struct io_object_t io_object_new(void *ref, io_object_f proc, delete_f delete)
{
	struct inst_t *inst;

	inst = mem_alloc(sizeof(struct inst_t));
	*inst = (struct inst_t){ ref, proc, delete };

	return (struct io_object_t){ inst, &inst_iface };
}

/**
 * Process an I/O object.
 *   @object: The object.
 *   @output: The output device.
 */

_export
void io_object_proc(struct io_object_t object, struct io_output_t output)
{
	object.iface->proc(object.ref, output);
}

/**
 * Close an I/O object.
 *   @object: The object.
 */

_export
void io_object_delete(struct io_object_t object)
{
	object.iface->delete(object.ref);
}


/**
 * Process an instance.
 *   @inst: The instance.
 *   @output: The output.
 */

static void inst_proc(struct inst_t *inst, struct io_output_t output)
{
	inst->proc(inst->ref, output);
}

/**
 * Delete an instance.
 *   @inst: The instance.
 */

static void inst_delete(struct inst_t *inst)
{
	if(inst->delete != NULL)
		inst->delete(inst->ref);

	mem_free(inst);
}


/**
 * Create an I/O object for copying a path.
 *   @path: The path.
 *   &returns: The path.
 */

_export
struct io_object_t io_object_copy(const char *path)
{
	return (struct io_object_t){ str_dup(path), &copy_iface };
}

/**
 * Process a copy.
 *   @path: The path.
 *   @output: The output.
 */

static void copy_proc(char *path, struct io_output_t output)
{
	size_t nbytes;
	uint8_t buf[4096];
	struct io_input_t input;

	input = io_input_open(path);

	while((nbytes = io_input_read(input, buf, sizeof(buf))) != 0)
		io_output_writefull(output, buf, nbytes);

	io_input_close(input);
}


/**
 * Create an I/O object for copying a segment of a file.
 *   @file: The file.
 *   @offset: The offset.
 *   @nbytes: The number of bytes.
 *   &returns: The object.
 */

struct io_object_t io_file_segment(struct io_file_t file, uint64_t offset, uint64_t nbytes)
{
	struct segment_t *segment;

	segment = mem_alloc(sizeof(struct segment_t));
	*segment = (struct segment_t){ file, offset, nbytes };

	return (struct io_object_t){ segment, &segment_iface };
}

/**
 * Process a segment.
 *   @segment: The segment information.
 *   @output: The output.
 */

static void segment_proc(struct segment_t *segment, struct io_output_t output)
{
	io_output_segment(output, segment->file, segment->offset, segment->nbytes);
}
