#include "../../common.h"
#include <stdio.h>
#include "../../debug/exception.h"
#include "../../io/chunk.h"
#include "../../io/output.h"
#include "../../io/print.h"
#include "../env.h"
#include "../path.h"


/*
 * loca function declarations
 */

static void path_proc(struct io_output_t output, void *arg);


/**
 * Generate a chunk for an OS path.
 *   @id: The path identifier.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t _impl_os_path_chunk(enum os_path_e id)
{
	return (struct io_chunk_t){ path_proc, (void *)id };
}

/**
 * Processing callback for OS paths.
 *   @output: The output stream.
 *   @arg: The argument.
 */

static void path_proc(struct io_output_t output, void *arg)
{
	enum os_path_e id = (enum os_path_e)arg;

	switch(id) {
	case os_path_lib_e:
#ifdef BMAKE__PATH_LIB
		io_output_str(output, BMAKE__PATH_LIB);
#else
		io_output_str(output, "/usr/local/lib");
#endif
		break;

	case os_path_share_e:
#ifdef BMAKE__PATH_SHARE
		io_output_str(output, BMAKE__PATH_SHARE);
#else
		io_output_str(output, "/usr/local/share");
#endif
		break;

	case os_path_user_config_e:
		io_printf(output, "%s/.config", os_env_get("HOME"));
		break;

	default:
		_fatal("Invalid OS path lookup. ID not recognized.");
	}
}
