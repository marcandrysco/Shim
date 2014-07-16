#include "../common.h"
#include "queue.h"
#include "../debug/exception.h"
#include "../mem/manage.h"


/*
 * local function declarations
 */

static void *inst_delete(struct queue_inst_t *inst);
static struct queue_inst_t *inst_cast(struct queue_node_t *node);

static void ref_delete(void *ref, delete_f delete);


/**
 * Add a node to the queue.
 *   @root: The root.
 *   @node: The node.
 */

_export
void queue_root_add(struct queue_root_t *root, struct queue_node_t *node)
{
	if(root->tail != NULL)
		root->tail->next = node;
	else
		root->head = node;

	root->tail = node;
	node->next = NULL;
	//struct queue_node_t **tail = root->head ? &root->tail->next : &root->head;

	//node->next = NULL;
	//*tail = NULL;
}

/**
 * Remove a node from the queue.
 *   @root: The root.
 *   &returns: The node or null.
 */

_export
struct queue_node_t *queue_root_remove(struct queue_root_t *root)
{
	struct queue_node_t *node;

	node = root->head;
	if(node != NULL) {
		root->head = node->next;
		if(root->head == NULL)
			root->tail = NULL;
	}

	return node;
}


/**
 * Create an empty queue.
 *   @delete: Optional. Reference deletion.
 */

_export
struct queue_t queue_empty(delete_f delete)
{
	return (struct queue_t){ 0, QUEUE_ROOT_INIT, delete };
}

/**
 * Destroy a queue.
 *   @queue: The queue.
 */

_export
void queue_destroy(struct queue_t *queue)
{
	struct queue_node_t *cur, *next;

	for(cur = queue->root.head; cur != NULL; cur = next) {
		next = cur->next;

		ref_delete(inst_delete(inst_cast(cur)), queue->delete);
	}
}


/**
 * Add a refrence to a queue.
 *   @queue: The queue.
 *   @ref: The reference.
 */

_export
void queue_add(struct queue_t *queue, void *ref)
{
	struct queue_inst_t *inst;

	inst = mem_alloc(sizeof(struct queue_inst_t));
	inst->ref = ref;

	queue->len++;
	queue_root_add(&queue->root, &inst->node);
}

/**
 * Remove a reference from a queue.
 *   @queue: The queue.
 *   &returns: The reference.
 */

_export
void *queue_remove(struct queue_t *queue)
{
	struct queue_node_t *node;

	node = queue_root_remove(&queue->root);
	if(node == NULL)
		return NULL;

	queue->len--;
	return inst_delete(inst_cast(node));
}

/**
 * Purge a reference from the queue.
 *   @queue: The queue.
 */

_export
void queue_purge(struct queue_t *queue)
{
	void *ref;

	ref = queue_remove(queue);
	if(ref == NULL)
		throw("Attempted to purge an empty queue.");

	ref_delete(ref, queue->delete);
}


/**
 * Cast node to an instance.
 *   @node: The node.
 *   &returns: The instance.
 */

static struct queue_inst_t *inst_cast(struct queue_node_t *node)
{
	return (void *)node - offsetof(struct queue_inst_t, node);
}

/**
 * Delete an instance.
 *   @inst: The instance.
 *   &returns: The reference.
 */

static void *inst_delete(struct queue_inst_t *inst)
{
	void *ref;
	
	ref = inst->ref;
	mem_free(inst);

	return ref;
}


/**
 * Delete a reference.
 *   @ref: The reference.
 *   @delete: The deletion callback.
 */

static void ref_delete(void *ref, delete_f delete)
{
	if(delete)
		delete(ref);
}
