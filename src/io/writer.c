#include "../common.h"
#include "writer.h"
#include "../mem/manage.h"
#include "../types/avltree.h"
#include "defs.h"
#include "output.h"


/**
 * Writer structure.
 *   @output: The output device.
 *   @tag: The current tag.
 *   @inst: The written instances.
 */

struct io_writer_t {
	struct io_output_t output;

	unsigned int tag;
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
static const struct inst_t *inst_cast_const(const struct avltree_node_t *node);
static int inst_compare_key(const void *key, const struct avltree_node_t *node, void *arg);
static int inst_compare_node(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg);


/**
 * Create a new I/O writer.
 *   @output: The output device.
 *   &return: The writer.
 */

_export
struct io_writer_t *io_writer_new(struct io_output_t output)
{
	struct io_writer_t *writer;

	writer = mem_alloc(sizeof(struct io_writer_t));
	writer->output = output;
	writer->tag = 1;
	writer->inst = NULL;

	return writer;
}

/**
 * Delete an I/O writer.
 *   @writer: The writer.
 */

_export
void io_writer_delete(struct io_writer_t *writer)
{
	struct avltree_iter_t iter;
	struct avltree_node_t *node;

	avltree_node_iter_init(&iter, writer->inst);
	while((node = avltree_node_iter_next(&iter)) != NULL)
		mem_free(inst_cast(node));

	mem_free(writer);
}


/**
 * Retrieve the output device of the writer.
 *   @writer: The writer.
 *   &returns: The output.
 */

_export
struct io_output_t io_write_output(struct io_writer_t *writer)
{
	return writer->output;
}


/**
 * Save a reference to the writer.
 *   @writer: The writer.
 *   @ref: The reference.
 *   @save: The save callback.
 */

_export
void io_writer_save(struct io_writer_t *writer, void *ref, io_save_f save, void *arg)
{
	struct inst_t *inst;
	struct avltree_node_t *node;

	node = avltree_node_lookup(writer->inst, ref, inst_compare_key, NULL);
	if(node == NULL) {
		inst = mem_alloc(sizeof(struct inst_t));
		inst->ref = ref;
		inst->tag = writer->tag++;

		avltree_node_insert(&writer->inst, &inst->node, inst_compare_node, NULL);

		io_output_uint32(writer->output, 0);
		save(ref, writer, arg);
	}
	else
		inst = inst_cast(node);

	io_output_uint32(writer->output, inst->tag);
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

/**
 * Cast a const node to an instance.
 *   @node: The node.
 *   &returns: The instance.
 */

static const struct inst_t *inst_cast_const(const struct avltree_node_t *node)
{
	return (void *)node - offsetof(struct inst_t, node);
}

/**
 * Coimpare instances by key.
 *   @key: The key.
 *   @node: The node.
 *   @arg: Unused argument.
 *   &returns: Their order.
 */

static int inst_compare_key(const void *key, const struct avltree_node_t *node, void *arg)
{
	const struct inst_t *inst = inst_cast_const(node);

	if(key > inst->ref)
		return 1;
	else if(key < inst->ref)
		return -1;
	else
		return 0;
}

/**
 * Coimpare instances by node.
 *   @n1: The first node.
 *   @n2: The second node.
 *   @arg: Unused argument.
 *   &returns: Their order.
 */

static int inst_compare_node(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg)
{
	const struct inst_t *i1 = inst_cast_const(n1), *i2 = inst_cast_const(n2);

	if(i1->ref > i2->ref)
		return 1;
	else if(i1->ref < i2->ref)
		return -1;
	else
		return 0;
}
