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


String2D directory_strings(const char *name)
{
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	String2D strings = { NULL, 0, 1 };

	strings.list = malloc(sizeof(char) * strings.len);
	assert(strings.list);

	dir = opendir("KIU:/");
	while ((entry = readdir(dir)) != NULL) {
		if (strings.len == strings.cap) {
			strings.cap *= 2;
			strings.list = realloc(strings.list, sizeof(char) * strings.cap);
			assert(strings.list);
		}
		strings.list[strings.len] = strdup(entry->d_name);
		assert(strings.list[strings.len]);
		strings.len++;
	}
	closedir(dir);

	return strings;
}

