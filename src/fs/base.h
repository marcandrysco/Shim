#ifndef FS_BASE_H
#define FS_BASE_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * base filesystem function declarations
 */

bool fs_exists(const char *path);
bool fs_isdir(const char *path);

void fs_mkdir(const char *path);
void fs_rmdir(const char *path);
void fs_rmfile(const char *path);
void fs_clear(const char *path);

struct iter_t fs_lsdir(const char *path);

char *fs_mktmpdir(const char *prefix);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
