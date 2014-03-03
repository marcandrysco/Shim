#ifndef OS_PATH_H
#define OS_PATH_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Path enumerator.
 *   @os_path_lib_e: The library directory.
 *   @os_path_share_e: The shared resource directory.
 *   @os_path_user_config_e: The user configuration directory.
 */

enum os_path_e {
	os_path_lib_e,
	os_path_share_e,
	os_path_user_config_e
};


/*
 * operating system path function declarations
 */

struct io_chunk_t os_path_chunk(enum os_path_e id);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
