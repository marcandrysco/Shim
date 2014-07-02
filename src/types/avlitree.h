#ifndef TYPES_AVLITREE_H
#define TYPES_AVLITREE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * AVL index tree definitions.
 *   @AVLITREE_MAX_HEIGHT: The maximum height of an AVL tree.
 *   @AVLITREE_NODE_INIT: An inline structure to initialize a node.
 */

#define AVLITREE_MAX_HEIGHT	48
#define AVLITREE_NODE_INIT	(struct avlitree_node_t){ 0, { NULL, NULL }, NULL, 1 }


/*
 * structure prototypes
 */

struct avlitree_t;
struct avlitree_node_t;
struct avlitree_ref_t;
struct avlitree_iter_t;


/**
 * Deletion callback for a node.
 *   @node: The current node.
 *   @arg: The callback argument.
 */

typedef void (*avlitree_delete_node_f)(struct avlitree_node_t *node, void *arg);


/**
 * Storage feor the AVL index tree.
 *   @root: A reference to the root node.
 *   @count: The number of nodes in the tree.
 *   @delete: The deletion function for nodes.
 */

struct avlitree_t {
	struct avlitree_node_t *root;

	unsigned int count;

	delete_f delete;
};

/**
 * AVL index tree node storage.
 *   @balance: The current balance of the node, between '-2' to '2'.
 *   @child, parent: The child and parent nodes.
 *   @count: The count of nodes below this node.
 */

struct avlitree_node_t {
	int8_t balance;
	struct avlitree_node_t *child[2], *parent;
	unsigned int count;
};

/**
 * AVL joint tree root structure.
 *   @node: The root node.
 */

struct avlitree_root_t {
	struct avlitree_node_t *node;
};

/**
 * AVL index tree reference storage.
 *   @ref: The value reference.
 *   @node: The node.
 */

struct avlitree_ref_t {
	void *ref;

	struct avlitree_node_t node;
};

/**
 * AVL index tree iterator storage.
 *   @i: The current level.
 *   @stack: The stack of nodes.
 *   @state: The stack of states per level.
 */

struct avlitree_iter_t {
	short i;
	struct avlitree_node_t *stack[AVLITREE_MAX_HEIGHT];
	uint8_t state[AVLITREE_MAX_HEIGHT];
};


/*
 * avl index tree node function declarations
 */

struct avlitree_root_t avlitree_root_empty();

struct avlitree_node_t *avlitree_root_first(struct avlitree_root_t *root);
struct avlitree_node_t *avlitree_root_last(struct avlitree_root_t *root);
struct avlitree_node_t *avlitree_root_get(struct avlitree_root_t *root, unsigned int index);

void avlitree_root_insert(struct avlitree_root_t *root, unsigned int index, struct avlitree_node_t *node);
struct avlitree_node_t *avlitree_root_slice(struct avlitree_root_t *root, unsigned int index);
struct avlitree_node_t *avlitree_root_set(struct avlitree_root_t *root, unsigned int index, struct avlitree_node_t *node);

struct avlitree_node_t *avlitree_node_prev(struct avlitree_node_t *node);
struct avlitree_node_t *avlitree_node_next(struct avlitree_node_t *node);

struct avlitree_node_t *avlitree_node_first(struct avlitree_node_t *root);
struct avlitree_node_t *avlitree_node_last(struct avlitree_node_t *root);
struct avlitree_node_t *avlitree_node_get(struct avlitree_node_t *root, unsigned int index);
struct avlitree_node_t *avlitree_node_set(struct avlitree_node_t **root, unsigned int index, struct avlitree_node_t *node);
unsigned int avlitree_node_index(struct avlitree_node_t *node);

void avlitree_node_insert(struct avlitree_node_t **root, unsigned int index, struct avlitree_node_t *node);
struct avlitree_node_t *avlitree_node_slice(struct avlitree_node_t **root, unsigned int index);
void avlitree_node_clear(struct avlitree_node_t *root, avlitree_delete_node_f delete, void *arg);

struct avlitree_iter_t avlitree_node_iter_new(struct avlitree_node_t *root);
struct avlitree_node_t *avlitree_node_iter_next(struct avlitree_iter_t *iter);

/**
 * avl index tree function declarations
 */

void avlitree_init(struct avlitree_t *tree, delete_f delete);
struct avlitree_t avlitree_empty(delete_f delete);
struct avlitree_t *avlitree_new(delete_f delete);
void avlitree_destroy(struct avlitree_t *tree);
void avlitree_delete(struct avlitree_t *tree);

void *avlitree_get(const struct avlitree_t *tree, unsigned int index);
void *avlitree_set(struct avlitree_t *tree, unsigned int index, void *ref);
void *avlitree_set(struct avlitree_t *tree, unsigned int index, void *ref);

void avlitree_insert(struct avlitree_t *tree, unsigned int index, void *ref);
void *avlitree_slice(struct avlitree_t *tree, unsigned int index);
void avlitree_erase(struct avlitree_t *tree, unsigned int index);
void avlitree_clear(struct avlitree_t *tree);

void avlitree_append(struct avlitree_t *tree, void *ref);
void avlitree_prepend(struct avlitree_t *tree, void *ref);

struct avlitree_iter_t avlitree_iter_new(const struct avlitree_t *tree);
void *avlitree_iter_next(struct avlitree_iter_t *iter);
struct avlitree_ref_t *avlitree_iter_next_ref(struct avlitree_iter_t *iter);

struct iter_t avlitree_valiter_new(const struct avlitree_t *tree);
struct iter_t avlitree_refiter_new(const struct avlitree_t *tree);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
