#include "archive.h"


void archive_handle_errors(Result res, const char *name)
{
	if (!R_SUCCEEDED(res)) {
		printf("Failed to mount %s.\n", name);
		archiveUnmount(name);
		sleep(20);
		gfxExit();
	}
}


void archive_open(const char *name)
{
	uint32_t data[3] = { MEDIATYPE_SD, 0x00030100, 0x00040000 };
	FS_Path path = { PATH_BINARY, 12, (const void*)data };

	Result res = archiveMount(ARCHIVE_USER_SAVEDATA, path, name);
	archive_handle_errors(res, name);
}


void archive_close(const char *name)
{
	archiveUnmount(name);
}

