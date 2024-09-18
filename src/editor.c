#include "editor.h"
#ifdef TEST_BUILD
#define SAVE_PATH "test/input/editor/"
#else
#define SAVE_PATH "KIU:/"
#endif


Editor editor_init(void)
{
	Editor editor;

	editor.ctx.save_loaded = false;
	editor.ctx.files = NULL;
	editor.ctx.files_len = 0;
	editor.ctx.key_down_old = 0;

	menu_init(&editor.menu, "Kid Icarus Uprising Save Editor", editor_switch_submenu);
	set_submenu_desc(editor.menu.root, "Choose a save file.");

	editor.ctx.files = get_save_strings(SAVE_PATH, &editor.ctx.files_len);

	for (int i = 0; i < editor.ctx.files_len; i++)
		add_submenu_option(editor.menu.root, editor.ctx.files[i], load_save);
	
	return editor;
}


int comp_string(const void *string1, const void *string2)
{
	return strcmp(*(const char**)string1, *(const char**)string2);
}


char **get_save_strings(const char *dir_path, uint32_t *len)
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


void build_weapon_menus(Submenu *submenu, SaveFile save)
{
	Submenu *curr = NULL;
	const char *weapon_name = NULL;

	for (int i = 0; i < save.weapons.len; i++) {
		weapon_name = save.weapons.array[i]->name;
		curr = add_submenu_option(submenu, weapon_name, switch_submenu);
		curr->data = save.weapons.array[i];
	}
}


void editor_switch_submenu(void *menu_ptr, void *context_ptr)
{
	Menu *menu = (Menu*)menu_ptr;
	MenuContext *context = (MenuContext*)context_ptr;
	uint32_t k_down = context->key_down_old;

	if (k_down & BIT(1) && menu->curr->prev) {
		menu->curr = menu->curr->prev;
		return;
	}

	if (k_down & BIT(0) && menu->curr->len > 0) {
		menu->curr = menu->curr->options[menu->curr->selected];
	}
}


void load_save(void *menu_ptr, void *context_ptr)
{
	Menu *menu = (Menu*)menu_ptr;
	MenuContext *context = (MenuContext*)context_ptr;

	// If the save file is loaded, simply swap the page.
	if (context->save_loaded) {
		switch_submenu(menu_ptr, &context->key_down);
		return;
	}

	context->file_index = menu->curr->selected;
	// Bug occurs here
	savefile_init(&context->save, context->files[context->file_index]);

	build_weapon_menus(menu->curr, context->save);
	context->save_loaded = true;
	switch_submenu(menu, &context->key_down);
}
