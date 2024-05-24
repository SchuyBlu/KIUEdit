#include "archive.h"


void archive_handle_errors(Result res, const char *name)
{
	if (!R_SUCCEEDED(res)) {
		printf("Failed to mount %s.\n", name);
		archiveUnmount(name);
		sleep(5);
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


char **directory_strings(const char *name)
{
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	char **list;
	int cap = 1, len = 0;

	list = malloc(sizeof(char) * cap);
	assert(list);

	dir = opendir("KIU:/");
	while ((entry = readdir(dir)) != NULL) {
		if (len == cap) {
			cap *= 2;
			list = realloc(list, sizeof(char) * cap);
			assert(list);
		}
		list[len] = strdup(entry->d_name);
		assert(list[len]);
		len++;
	}
	closedir(dir);

	return list;
}

