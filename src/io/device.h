#ifndef IO_DEVICE_H
#define IO_DEVICE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * blank function declarations
 */

bool io_blank_ctrl(void *ref, unsigned int id, void *data);
void io_blank_close(void *ref);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
