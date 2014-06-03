#include "../common.h"
#include "reader.h"
#include "../debug/exception.h"
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
static const struct inst_t *inst_cast_const(const struct avltree_node_t *node);
static int inst_compare_key(const void *key, const struct avltree_node_t *node, void *arg);
static int inst_compare_node(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg);


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

	iter = avltree_node_iter_begin(reader->inst);
	while((node = avltree_node_iter_next_depth(&iter)) != NULL)
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
 * Load a reference from the reader.
 *   @reader: The reader.
 *   @load: The load callback.
 *   &returns: The reference.
 */

_export
void *io_reader_load(struct io_reader_t *reader, io_load_f load, void *arg)
{
	unsigned int tag;
	struct inst_t *inst;

	tag = io_input_uint32(reader->input);
	if(tag == 0) {
		inst = mem_alloc(sizeof(struct inst_t));
		inst->ref = load(reader, arg);
		inst->tag = io_input_uint32(reader->input);

		avltree_node_insert(&reader->inst, &inst->node, inst_compare_node, NULL);
	}
	else {
		struct avltree_node_t *node;

		node = avltree_node_lookup(reader->inst, &tag, inst_compare_key, NULL);
		if(node == NULL)
			throw("Corrupt data.");

		inst = inst_cast(node);
	}

	return inst->ref;
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
	unsigned int tag = *(unsigned int *)key;

	if(tag > inst->tag)
		return 1;
	else if(tag < inst->tag)
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

	if(i1->tag > i2->tag)
		return 1;
	else if(i1->tag < i2->tag)
		return -1;
	else
		return 0;
}
