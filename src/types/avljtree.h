#ifndef TYPES_AVLJTREE_H
#define TYPES_AVLJTREE_H

/*
 * project headers
 */

#include "avltree.h"
#include "avlitree.h"

/*
 * sanity check
 */

#if AVLTREE_MAX_HEIGHT != AVLITREE_MAX_HEIGHT
#	error "Max height of AVL trees and AVL index trees do not match."
#endif


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * AVL joint tree definitions.
 *   @AVLJTREE_MAX_HEIGHT: The maximum height of an AVL tree.
 *   @AVLJTREE_NODE_INIT: An inline structure to initialize a node.
 */

#define AVLJTREE_MAX_HEIGHT	AVLTREE_MAX_HEIGHT
#define AVLJTREE_NODE_INIT	(struct avljtree_node_t){ AVLTREE_NODE_INIT, AVLITREE_NODE_INIT }


/*
 * structure prototypes
 */

struct avljtree_t;
struct avljtree_node_t;
struct avljtree_ref_t;
struct avljtree_iter_t;


/**
 * Deletion callback for a node.
 *   @node: The current node.
 *   @arg: The callback argument.
 */

typedef void (*avljtree_delete_node_f)(struct avljtree_node_t *node, void *arg);


/**
 * AVL joint tree structure.
 *   @keyroot: The keyed root node.
 *   @idxroot: The indexeded root node.
 *   @count: The number of nodes.
 *   @compare: The comparison function.
 *   @delete: The value deletion function.
 */

struct avljtree_t {
	struct avljtree_node_t *keyroot;
	struct avljtree_node_t *idxroot;

	unsigned int count;

	compare_f compare;
	delete_f delete;
};

/**
 * AVL joint tree node storage.
 *   @key: The key-based node.
 *   @index: The index-based node.
 */

struct avljtree_node_t {
	struct avltree_node_t key;
	struct avlitree_node_t index;
};

/**
 * AVL joint tree root structure.
 *   @bykey, byidx: By key and index.
 */

struct avljtree_root_t {
	struct avljtree_node_t *bykey, *byidx;
};

/**
 * AVL joint tree reference storage.
 *   @key: The key reference.
 *   @ref: The value reference.
 *   @node: The node.
 */

struct avljtree_ref_t {
	const void *key;
	void *ref;

	struct avljtree_node_t node;
};

/**
 * AVL joint tree key iterator.
 *   @keyiter: The base AVL tree iterator.
 */

struct avljtree_keyiter_t {
	struct avltree_iter_t keyiter;
};

/**
 * AVL joint tree index iterator.
 *   @keyiter: The base AVL index tree iterator.
 */

struct avljtree_idxiter_t {
	struct avlitree_iter_t idxiter;
};


/*
 * avl joint tree root function declarations
 */

struct avljtree_root_t avljtree_root_empty();

struct avljtree_node_t *avljtree_root_idxfirst(struct avljtree_root_t *root);
struct avljtree_node_t *avljtree_root_idxlast(struct avljtree_root_t *root);

struct avljtree_node_t *avljtree_root_get(struct avljtree_root_t *root, unsigned int index);

void avljtree_root_prepend(struct avljtree_root_t *root, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
void avljtree_root_append(struct avljtree_root_t *root, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
void avljtree_root_insert(struct avljtree_root_t *root, unsigned int index, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg);

struct avljtree_node_t *avljtree_root_remove(struct avljtree_root_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg);

void avljtree_root_keyiter_init(struct avljtree_keyiter_t *iter, struct avljtree_root_t *root);
struct avljtree_node_t *avljtree_root_keyiter_next(struct avljtree_keyiter_t *iter);
struct avljtree_node_t *avljtree_root_keyiter_next_depth(struct avljtree_keyiter_t *iter);

void avljtree_root_idxiter_init(struct avljtree_idxiter_t *iter, struct avljtree_root_t *root);
struct avljtree_node_t *avljtree_root_idxiter_next(struct avljtree_idxiter_t *iter);

/*
 * avl joint tree node function declarations
 */

struct avljtree_node_t *avljtree_node_keyfirst(struct avljtree_node_t *root);
struct avljtree_node_t *avljtree_node_keylast(struct avljtree_node_t *root);
struct avljtree_node_t *avljtree_node_idxfirst(struct avljtree_node_t *root);
struct avljtree_node_t *avljtree_node_idxlast(struct avljtree_node_t *root);
struct avljtree_node_t *avljtree_node_lookup(struct avljtree_node_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg);
struct avljtree_node_t *avljtree_node_get(struct avljtree_node_t *root, unsigned int index);
unsigned int avljtree_node_index(struct avljtree_node_t *node);

void avljtree_node_prepend(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
void avljtree_node_append(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
void avljtree_node_insert(struct avljtree_node_t **keyroot, struct avljtree_node_t **iroot, unsigned int index, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
struct avljtree_node_t *avljtree_node_set(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, unsigned int index, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
struct avljtree_node_t *avljtree_node_replace(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, struct avljtree_node_t *node, const void *key, avltree_compare_nodekey_f compare_k, void *arg_k, avltree_compare_nodenode_f compare_n, void *arg_n);
struct avljtree_node_t *avljtree_node_slice(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, unsigned int index, avltree_compare_nodenode_f compare, void *arg);
struct avljtree_node_t *avljtree_node_remove(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, const void *key, avltree_compare_nodekey_f compare, void *arg);
void avljtree_node_clear(struct avljtree_node_t *root, avljtree_delete_node_f delete, void *arg);

struct avljtree_keyiter_t avljtree_node_keyiter_new(struct avljtree_node_t *root);
struct avljtree_node_t *avljtree_node_keyiter_next(struct avljtree_keyiter_t *iter);

void avljtree_node_idxiter_init(struct avljtree_idxiter_t *iter, struct avljtree_node_t *root);
struct avljtree_node_t *avljtree_node_idxiter_next(struct avljtree_idxiter_t *iter);

/*
 * avl joint tree function declarations
 */

struct avljtree_t avljtree_empty(compare_f compare, delete_f delete);
void avljtree_init(struct avljtree_t *tree, compare_f compare, delete_f delete);
void avljtree_destroy(struct avljtree_t *tree);
void avljtree_delete(struct avljtree_t *tree);

void *avljtree_first(const struct avljtree_t *tree);
void *avljtree_last(const struct avljtree_t *tree);
void *avljtree_lookup(const struct avljtree_t *tree, const void *key);
void *avljtree_get(const struct avljtree_t *tree, unsigned int index);
unsigned int avljtree_index(const struct avljtree_t *tree, const void *key);

void avljtree_insert(struct avljtree_t *tree, unsigned int index, const void *key, void *ref);
void *avljtree_set(struct avljtree_t *tree, unsigned int index, const void *key, void *ref);
void *avljtree_replace(struct avljtree_t *tree, const void *orig, const void *key, void *ref);
void avljtree_displace(struct avljtree_t *tree, const void *orig, const void *key, void *ref);
void *avljtree_slice(struct avljtree_t *tree, unsigned int index);
void avljtree_erase(struct avljtree_t *tree, unsigned int index);
void *avljtree_remove(struct avljtree_t *tree, const void *key);
void avljtree_purge(struct avljtree_t *tree, const void *key);
void avljtree_clear(struct avljtree_t *tree);

void avljtree_append(struct avljtree_t *tree, const void *key, void *ref);
void avljtree_prepend(struct avljtree_t *tree, const void *key, void *ref);

struct avljtree_keyiter_t avljtree_keyiter_begin(const struct avljtree_t *tree);
void *avljtree_keyiter_next(struct avljtree_keyiter_t *iter);
void *avljtree_keyiter_next_key(struct avljtree_keyiter_t *iter);
struct avljtree_ref_t *avljtree_keyiter_next_ref(struct avljtree_keyiter_t *iter);

struct iter_t avljtree_keyiter_vals_new(struct avljtree_t *tree);
struct iter_t avljtree_keyiter_keys_new(struct avljtree_t *tree);
struct iter_t avljtree_keyiter_refs_new(struct avljtree_t *tree);

void avljtree_idxiter_init(struct avljtree_idxiter_t *iter, const struct avljtree_t *tree);
void *avljtree_idxiter_next(struct avljtree_idxiter_t *iter);
void *avljtree_idxiter_next_key(struct avljtree_idxiter_t *iter);
struct avljtree_ref_t *avljtree_idxiter_next_ref(struct avljtree_idxiter_t *iter);

struct iter_t avljtree_idxiter_vals_new(struct avljtree_t *tree);
struct iter_t avljtree_idxiter_keys_new(struct avljtree_t *tree);
struct iter_t avljtree_idxiter_refs_new(struct avljtree_t *tree);


/**
 * Cast an AVL tree node to an AVL joint tree node.
 *   @node: The AVL tree node.
 *   &returns: The AVL joint tree node.
 */

static inline struct avljtree_node_t *avljtree_cast_bykey(struct avltree_node_t *node)
{
	return (void *)node - offsetof(struct avljtree_node_t, key);
}

/**
 * Constant cast an AVL tree node to an AVL joint tree node.
 *   @node: The AVL tree node.
 *   &returns: The AVL joint tree node.
 */

static inline const struct avljtree_node_t *avljtree_cast_bykey_const(const struct avltree_node_t *node)
{
	return (const void *)node - offsetof(struct avljtree_node_t, key);
}

/**
 * Cast an AVL index tree node to an AVL joint tree node.
 *   @node: The AVL index tree node.
 *   &returns: The AVL joint tree node.
 */

static inline struct avljtree_node_t *avljtree_cast_byidx(struct avlitree_node_t *node)
{
	return (void *)node - offsetof(struct avljtree_node_t, index);
}

/**
 * Constant cast an AVL index tree node to an AVL joint tree node.
 *   @node: The AVL index tree node.
 *   &returns: The AVL joint tree node.
 */

static inline const struct avljtree_node_t *avljtree_cast_byidx_const(const struct avlitree_node_t *node)
{
	return (const void *)node - offsetof(struct avljtree_node_t, index);
}


/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
