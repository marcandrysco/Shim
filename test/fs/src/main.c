#include "common.h"


/**
 * Direct testing
 *   &returns: True of success, false on failure.
 */

bool test_fs_dir()
{
	char *path;

	printf("testing directories... ");

	fs_mkdir("./tmp-test");
	if(!fs_isdir("./tmp-test"))
		printf("failed\n"), sys_exit(1);

	fs_rmdir("./tmp-test");

	path = fs_mktmpdir("./tmp-");
	if(!fs_isdir(path))
		printf("failed\n"), sys_exit(1);

	fs_rmdir(path);
	mem_free(path);

	printf("okay\n");

	return true;
}

/**
 * Main entry point.
 *   @argc: The number of arguments.
 *   @argv: The argument array.
 *   &returns: The return status code.
 */

int main(int argc, char *argv[])
{
	bool suc = true;

	suc &= test_fs_dir();

	return suc ? 0 : 1;
}
