#include "../common.h"
#include "path.h"
#include "../io/chunk.h"


/*
 * implementation function declaratoins
 */

struct io_chunk_t _impl_os_path_chunk(enum os_path_e id);


/**
 * Generate a chunk for an OS path.
 *   @id: The path identifier.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t os_path_chunk(enum os_path_e id)
{
	return _impl_os_path_chunk(id);
}
