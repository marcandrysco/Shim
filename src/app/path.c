#include "../common.h"
#include "path.h"
#include "../io/chunk.h"
#include "../os/path.h"


/**
 * Retrieve the application path.
 *   @id: The path identifier.
 *   @app: The application ID.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t app_path_chunk(const char *app, enum os_path_e id)
{
	return os_path_chunk(id);
}
