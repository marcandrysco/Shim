#include "../common.h"
#include "path.h"
#include "../debug/exception.h"
#include "../io/chunk.h"
#include "../io/print.h"
#include "../os/path.h"


/*
 * local function declarations
 */

static void chunk_lib(struct io_output_t output, void *arg);
static void chunk_share(struct io_output_t output, void *arg);
static void chunk_user_config(struct io_output_t output, void *arg);


/**
 * Retrieve the application path.
 *   @id: The path identifier.
 *   @app: The application ID.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t app_path_chunk(const char *app, enum os_path_e id)
{
	switch(id) {
	case os_path_lib_e: return (struct io_chunk_t){ chunk_lib, (void *)app };
	case os_path_share_e: return (struct io_chunk_t){ chunk_share, (void *)app };
	case os_path_user_config_e: return (struct io_chunk_t){ chunk_user_config, (void *)app };
	}
	
	throw("Invalid path id.");
}

/**
 * Chunk callback for the library path.
 *   @output: The output.
 *   @arg: The applicaiton ID argument.
 */

static void chunk_lib(struct io_output_t output, void *arg)
{
	io_printf(output, "%C/%s", os_path_chunk(os_path_lib_e), (const char *)arg);
}

/**
 * Chunk callback for the share path.
 *   @output: The output.
 *   @arg: The applicaiton ID argument.
 */

static void chunk_share(struct io_output_t output, void *arg)
{
	io_printf(output, "%C/%s", os_path_chunk(os_path_share_e), (const char *)arg);
}

/**
 * Chunk callback for the user configuration path.
 *   @output: The output.
 *   @arg: The applicaiton ID argument.
 */

static void chunk_user_config(struct io_output_t output, void *arg)
{
	io_printf(output, "%C/%s", os_path_chunk(os_path_user_config_e), (const char *)arg);
}
