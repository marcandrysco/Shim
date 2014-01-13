#include "../../common.h"
#include <unistd.h>
#include "../../debug/exception.h"
#include "../../mem/manage.h"
#include "../../types/avltree.h"
#include "../../types/compare.h"


/**
 * Synchronization structure.
 *   @pipe: The signal pipe.
 *   @fdset: The set of file descriptors.
 */

struct sys_sync_t {
	int pipe[2];

	struct avltree_t fdset;
};


/**
 * Signal structure.
 *   @syncset: The set of synchronization structure.
 *   @condset: The set of condition variables.
 */

struct sys_signal_t {
	struct avltree_t syncset;
	struct avltree_t condset;
};


/**
 * Create a new sychronization structure.
 *   &returns: The sync.
 */

_export
struct sys_sync_t *_impl_sys_sync_new()
{
	struct sys_sync_t *sync;

	sync = mem_alloc(sizeof(struct sys_sync_t));
	if(pipe(sync->pipe) < 0)
		throw("Failed to created synchronization pipe.");

	avltree_init(&sync->fdset, compare_int, mem_free);
	
	return sync;
}

/**
 * Delete a synchronization structure.
 *   @sync: The sync.
 */

_export
void _impl_sys_sync_delete(struct sys_sync_t *sync)
{
	close(sync->pipe[0]);
	close(sync->pipe[1]);

	avltree_destroy(&sync->fdset);
	mem_free(sync);
}


/**
 * Add a file descriptor to the synchronization structure.
 *   @sync: The sync.
 *   @fd: The file descriptor.
 */

_export
void _impl_sys_sync_add(struct sys_sync_t *sync, int fd)
{
	int *copy;

	copy = mem_alloc(sizeof(int));
	*copy = fd;

	avltree_insert(&sync->fdset, copy, copy);
}

/**
 * Purge a file descriptor from the synchronization structure.
 *   @sync: The sync.
 *   @fd: The file descriptor.
 */

_export
void _impl_sys_sync_remove(struct sys_sync_t *sync, int fd)
{
	avltree_purge(&sync->fdset, &fd);
}
