#include "editor.h"
#ifdef TEST_BUILD
#define SAVE_PATH "test/input/editor"
#else
#define SAVE_PATH "KIU:/"
#endif


void editor_init(Editor *editor)
{
	Submenu *option = NULL;

	editor->ctx.save_loaded = false;
	editor->ctx.files = NULL;
	editor->ctx.files_len = 0;
	editor->ctx.key_down_old = 0;

	menu_init(&editor->menu, "Kid Icarus Uprising Save Editor", editor_switch_submenu);
	set_submenu_desc(editor->menu.root, "Choose a save file.");


	editor->ctx.files = get_save_strings(SAVE_PATH, &editor->ctx.files_len);

	for (int i = 0; i < editor->ctx.files_len; i++) {
		option = add_submenu_option(editor->menu.root, editor->ctx.files[i], load_save);
		option->data = &editor->ctx;
	}
}

void destroy_editor(Editor *editor)
{
	destroy_menu(&editor->menu);
	destroy_savefile(&editor->ctx.save);
	for (int i = 0; i < editor->ctx.files_len; i++) {
		free(editor->ctx.files[i]);
	}
	free(editor->ctx.files);
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
	char *filename = NULL;

	list = malloc(sizeof(char*) * cap);
	assert(list);

	dir = opendir(dir_path);
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(".", entry->d_name) == 0 ||
			strcmp("..", entry->d_name) == 0 ||
			strcmp("00.SAV", entry->d_name) == 0)
			continue;

		if (*len == cap) {
			cap *= 2;
			list = realloc(list, sizeof(char*) * cap);
			assert(list);
		}

		filename = malloc(sizeof(char) * (strlen("KIU:/") + strlen(entry->d_name) + 1));
		assert(filename);

		strcpy(filename, "KIU:/");
		strcat(filename, entry->d_name);

		list[*len] = strdup(filename);
		assert(list[*len]);

		free(filename);

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
		curr = add_submenu_option(submenu, weapon_name, editor_switch_submenu);
		curr->data = save.weapons.array[i];
	}
}


void editor_switch_submenu(void *menu_ptr, void *context_ptr)
{
    Menu *menu = (Menu*)menu_ptr;
    MenuContext *context = (MenuContext*)context_ptr;
	Submenu *next = NULL;

    uint32_t k_down = context->key_down;  // ← use current frame, not key_down_old

    if ((k_down & BIT(1)) && menu->curr->prev) {  // B = back
        menu->curr = menu->curr->prev;
		menu->curr->view_top = 0;
		menu->curr->selected = 0;
        return;
    }

    if ((k_down & BIT(0)) && menu->curr->len > 0) {  // A = enter
        next = menu->curr->options[menu->curr->selected];

        // switch page immediately so print shows the new page this frame
        menu->curr = next;
		menu->curr->view_top = 0;
		menu->curr->selected = 0;

        // if this child has its own action (e.g., load_save), run it now
        if (next->action && next->action != editor_switch_submenu) {
            next->action(menu_ptr, context_ptr);
        }
    }
}


void load_save(void *menu_ptr, void *context_ptr)
{
	Menu *menu = (Menu*)menu_ptr;
	MenuContext *context = (MenuContext*)context_ptr;
	Submenu *selected = menu->curr, *save_page = menu->curr->prev, *old = NULL;

	// If a save is already loaded, first destroy the old save data,
	// and the old menu data.
	if (context->save_loaded) {
		// Note that file_index points to the most recently loaded save.
		destroy_savefile(&context->save);
		old = save_page->options[context->file_index];

		// Destroy old page
		for (int i = 0; i < old->len; i++) {
			_destroy_menu(old->options[i]);
			old->options[i] = NULL;
		}

		// Free the list of options on the old page
		free(old->options);
		old->options = NULL;

		// Reset the structure of the old page
		old->len = 0;
		old->cap = 1;
		old->selected = 0;
		old->dealloc_idx = 0;
		old->view_top = 0;
		old->options = NULL;
		old->action = load_save;

		context->save_loaded = false;
	}
	// Load the menu context regarding the saves, and load the save
	context->file_index = selected->prev->selected;
	savefile_init(&context->save, context->files[context->file_index]);

	build_weapon_menus(selected, context->save);

	selected->action = editor_switch_submenu;
	context->save_loaded = true;
}
