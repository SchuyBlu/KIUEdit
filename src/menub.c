#include "menub.h"
#ifdef TEST_BUILD
#define SAVE_PATH "test/input/menub/"
#else
#define SAVE_PATH "KIU:/"
#endif


Menu save_menu_init(void)
{
	Menu menu;
	char **files = NULL;
	int len = 0;

	menu_init(&menu, "Kid Icarus Uprising Save Editor", switch_submenu);
	set_submenu_desc(menu.root, "Choose a save file.");

	files = get_save_strings(SAVE_PATH, &len);
	for (int i = 0; i < len; i++)
		add_submenu_option(menu.root, files[i], switch_submenu);

	return menu;
}


int comp_string(const void *string1, const void *string2)
{
	return strcmp(*(const char**)string1, *(const char**)string2);
}


char **get_save_strings(const char *dir_path, int *len)
{
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	char **list = NULL;
	int cap = 1; *len = 0;

	list = malloc(sizeof(char*) * cap);
	assert(list);

	dir = opendir(dir_path);
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(".", entry->d_name) == 0 || 
			strcmp("..", entry->d_name) == 0)
			continue;

		if (*len == cap) {
			cap *= 2;
			list = realloc(list, sizeof(char*) * cap);
			assert(list);
		}
		list[*len] = strdup(entry->d_name);
		assert(list[*len]);
		(*len)++;
	}
	closedir(dir);

	qsort(list, *len, sizeof(char*), comp_string);

	return list;
}

