#ifndef TYPES_QUEUE_H
#define TYPES_QUEUE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Queue root structure.
 *   @head, tail: The and tail pointers.
 */

struct queue_root_t {
	struct queue_node_t *head, *tail;
};

/**
 * Queue node.
 *   @next: The next node.
 */

struct queue_node_t {
	struct queue_node_t *next;
};


/**
 * Queue structure.
 *   @len: The length.
 *   @root: The root.
 *   @delete: Delete callback.
 */

struct queue_t {
	unsigned int len;
	struct queue_root_t root;

	delete_f delete;
};

/**
 * Queue instance.
 *   @node: The node.
 *   @ref: The reference.
 */

struct queue_inst_t {
	struct queue_node_t node;

	void *ref;
};


/**
 * Initialize a queue root.
 *   &returns: The root.
 */

static inline struct queue_root_t queue_root()
{
	return (struct queue_root_t){ NULL, NULL };
}


/*
 * queue root definitions
 */

#define QUEUE_ROOT_INIT { NULL, NULL }

/*
 * queue root function declarations
 */

void queue_root_add(struct queue_root_t *root, struct queue_node_t *node);
struct queue_node_t *queue_root_remove(struct queue_root_t *root);

/*
 * queue function declarations
 */

struct queue_t queue_empty(delete_f delete);
void queue_destroy(struct queue_t *queue);

void queue_add(struct queue_t *queue, void *ref);
void *queue_remove(struct queue_t *queue);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
