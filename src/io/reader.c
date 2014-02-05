#include "../common.h"
#include "reader.h"
#include "../mem/manage.h"
#include "../types/avltree.h"
#include "defs.h"
#include "input.h"


/**
 * Reader structure.
 *   @input: The input device.
 *   @inst: The read instances.
 */

struct io_reader_t {
	struct io_input_t input;

	struct avltree_node_t *inst;
};

/**
 * Loaded instance structure.
 *   @ref: The reference.
 *   @tag: The tag.
 *   @node: The node.
 */

struct inst_t {
	void *ref;
	unsigned int tag;

	struct avltree_node_t node;
};


/*
 * local function declarations
 */

static struct inst_t *inst_cast(struct avltree_node_t *node);


/**
 * Create a new I/O reader.
 *   @input: The input device.
 *   &return: The reader.
 */

_export
struct io_reader_t *io_reader_new(struct io_input_t input)
{
	struct io_reader_t *reader;

	reader = mem_alloc(sizeof(struct io_reader_t));
	reader->input = input;
	reader->inst = NULL;

	return reader;
}

/**
 * Delete an I/O reader.
 *   @reader: The reader.
 */

_export
void io_reader_delete(struct io_reader_t *reader)
{
	struct avltree_iter_t iter;
	struct avltree_node_t *node;

	avltree_node_iter_init(&iter, reader->inst);
	while((node = avltree_node_iter_next(&iter)) != NULL)
		mem_free(inst_cast(node));

	mem_free(reader);
}


/**
 * Retrieve the associated input device.
 *   @reader: The reader.
 *   &returns: The input device.
 */

_export
struct io_input_t io_reader_input(struct io_reader_t *reader)
{
	return reader->input;
}


/**
 * Cast a node to an instance.
 *   @node: The node.
 *   &returns: The instance.
 */

static struct inst_t *inst_cast(struct avltree_node_t *node)
{
	return (void *)node - offsetof(struct inst_t, node);
}
