#ifndef APP_PATH_H
#define APP_PATH_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * type prototypes
 */

enum os_path_e;

/*
 * application path function declarations
 */

struct io_chunk_t app_path_chunk(const char *app, enum os_path_e id);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
