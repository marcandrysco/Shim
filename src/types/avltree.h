#ifndef AVLTREE_H
#define AVLTREE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * AVL tree definitions.
 *   @AVLTREE_MAX_HEIGHT: The maximum height of an AVL tree.
 *   @AVLTREE_NODE_INIT: An inline structure to initialize a node.
 */

#define AVLTREE_MAX_HEIGHT	48
#define AVLTREE_NODE_INIT	(struct avltree_node_t){ 0, { NULL, NULL } }


/*
 * structure prototypes
 */

struct avltree_t;
struct avltree_node_t;
struct avltree_ref_t;
struct avltree_iter_t;


/**
 * Comparison callback between a node and a key.
 *   @key: The input key.
 *   @node: The compared-to node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

typedef int (*avltree_compare_nodekey_f)(const void *key, const struct avltree_node_t *node, void *arg);

/**
 * Comparison callback between two nodes.
 *   @n1: The first node.
 *   @n2: The seconde node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

typedef int (*avltree_compare_nodenode_f)(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg);

/**
 * Deletion callback for a node.
 *   @node: The current node.
 *   @arg: The callback argument.
 */

typedef void (*avltree_delete_node_f)(struct avltree_node_t *node, void *arg);


/**
 * Iteration callback function on references.
 *   @ref: The reference.
 *   @arg: A user-specified argument.
 *   &returns: Non-zero to halt iteration, zero to continue.
 */

typedef short (*avltree_iterate_f)(void *ref, void *arg);

/**
 * Iteration callbcak function on keys.
 *   @ref: The reference.
 *   @arg: A user-specified argument.
 *   &returns: Non-zero to halt iteration, zero to continue.
 */

typedef short (*avltree_iterate_key_f)(const void *key, void *arg);

/**
 * Iteration callbcak function on reference structures.
 *   @ref: The reference.
 *   @arg: A user-specified argument.
 *   &returns: Non-zero to halt iteration, zero to continue.
 */

typedef short (*avltree_iterate_ref_f)(struct avltree_ref_t *ref, void *arg);


/**
 * AVL tree storage.
 *   @root: The root node.
 *   @count: The number of nodes.
 *   @compare: The comparison function.
 *   @delete: The value deletion function.
 */

struct avltree_t {
	struct avltree_node_t *root;

	unsigned int count;

	compare_f compare;
	delete_f delete;
};

/**
 * AVL tree node storage.
 *   @balance: The current balance of the node, between '-2' to '2'.
 *   @child: The child nodes.
 */

struct avltree_node_t {
	int8_t balance;
	struct avltree_node_t *child[2];
};

/**
 * AVL tree reference storage.
 *   @key: The key reference.
 *   @ref: The value reference.
 *   @node: The node.
 */

struct avltree_ref_t {
	const void *key;
	void *ref;

	struct avltree_node_t node;
};

/**
 * AVL tree iterator storage.
 *   @i: The current level.
 *   @stack: The stack of nodes.
 *   @state: The stack of states per level.
 */

struct avltree_iter_t {
	short i;
	struct avltree_node_t *stack[AVLTREE_MAX_HEIGHT];
	uint8_t state[AVLTREE_MAX_HEIGHT];
};


/*
 * avl tree node function declarations
 */

void *avltree_node_first(struct avltree_node_t *root);
void *avltree_node_last(struct avltree_node_t *root);
struct avltree_node_t *avltree_node_lookup(struct avltree_node_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg);

void avltree_node_insert(struct avltree_node_t **root, struct avltree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
struct avltree_node_t *avltree_node_remove(struct avltree_node_t **root, const void *key, avltree_compare_nodekey_f compare, void *arg);
void avltree_node_clear(struct avltree_node_t *root, avltree_delete_node_f delete, void *arg);

void avltree_node_iter_init(struct avltree_iter_t *iter, struct avltree_node_t *root);
struct avltree_node_t *avltree_node_iter_next(struct avltree_iter_t *iter);
struct avltree_node_t *avltree_node_iter_next_depth(struct avltree_iter_t *iter);

/*
 * avl tree function declarations
 */

void avltree_init(struct avltree_t *tree, compare_f compare, delete_f delete);
struct avltree_t *avltree_new(compare_f compare, delete_f delete);
void avltree_destroy(struct avltree_t *tree);
void avltree_delete(struct avltree_t *tree);

void *avltree_first(const struct avltree_t *tree);
void *avltree_last(const struct avltree_t *tree);
void *avltree_lookup(const struct avltree_t *tree, const void *key);

void avltree_insert(struct avltree_t *tree, const void *key, void *ref);
void *avltree_remove(struct avltree_t *tree, const void *key);
void avltree_purge(struct avltree_t *tree, const void *key);

void avltree_merge(struct avltree_t *dest, struct avltree_t *src);
void avltree_clear(struct avltree_t *tree);

void avltree_iter_init(struct avltree_iter_t *iter, const struct avltree_t *tree);
void *avltree_iter_next(struct avltree_iter_t *iter);
void *avltree_iter_next_key(struct avltree_iter_t *iter);
struct avltree_ref_t *avltree_iter_next_ref(struct avltree_iter_t *iter);

struct iter_t avltree_iter_new(const struct avltree_t *tree);
struct iter_t avltree_iter_keys_new(const struct avltree_t *tree);
struct iter_t avltree_iter_refs_new(const struct avltree_t *tree);

void avltree_iterate(const struct avltree_t *tree, avltree_iterate_f func, void *arg);
void avltree_iterate_keys(const struct avltree_t *tree, avltree_iterate_key_f func, void *arg);
void avltree_iterate_refs(const struct avltree_t *tree, avltree_iterate_ref_f func, void *arg);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
