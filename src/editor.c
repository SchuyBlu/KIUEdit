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

		// Necessary for testing.
#ifndef TEST_BUILD
		filename = malloc(sizeof(char) * (strlen(dir_path) + strlen(entry->d_name) + 1));
		assert(filename);
		strcpy(filename, dir_path);
		strcat(filename, entry->d_name);
#else
		filename = malloc(sizeof(char) * (strlen(entry->d_name) + 1));
		strcpy(filename, entry->d_name);
		assert(filename);
#endif

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
	char *new_weapon_name = NULL;
	uint32_t new_weapon_name_len = 0, value = 0;

	for (int i = 0; i < save.weapons.len; i++) {
		weapon_name = save.weapons.array[i]->name;
		curr = add_submenu_option(submenu, weapon_name, load_weapons);
		// makes room for <weapon_name> <weapon_value>v
		// Basically:
		// 1 char for space
		// 3 chars for value
		// 1 char for v
		// 1 char for null terminator
		new_weapon_name_len = sizeof(char) * (strlen(weapon_name) + 6);
		new_weapon_name = malloc(new_weapon_name_len);
		assert(new_weapon_name);

		value = weapon_get_value(save.weapons.array[i]);
		snprintf(new_weapon_name, new_weapon_name_len, "%s %luv", weapon_name, value);
		set_submenu_desc(curr, new_weapon_name);
		free(new_weapon_name);
		curr->data = save.weapons.array[i];
	}
}


void build_mod_menus(Submenu *submenu, SaveFile save)
{
	Weapon *weapon = (Weapon*)submenu->data;
	char *star_string = NULL;
	double ranged = 0.0, melee = 0.0;
	uint32_t star_string_len = 0;

	const char *mods[] = {
		weapon->mod1,
		weapon->mod2,
		weapon->mod3,
		weapon->mod4,
		weapon->mod5,
		weapon->mod6,
	};

	// Add a page to change the weapon
	add_submenu_option(submenu, "Change Weapon Type", editor_switch_submenu);

	// Add star values

	ranged = (double)weapon->ranged / 2.0;
	melee = (double)weapon->melee / 2.0;

	// Make room for: "Ranged Stars: <value>"
	star_string_len = sizeof(char) * (strlen("Ranged Stars: ") + 4);
	star_string = malloc(star_string_len);
	assert(star_string);

	snprintf(star_string, star_string_len, "Ranged Stars: %.1f", ranged);
	add_submenu_option(submenu, star_string, editor_switch_submenu);

	free(star_string);

	star_string_len = sizeof(char) * (strlen("Melee Stars: ") + 4);
	star_string = malloc(star_string_len);
	assert(star_string);
	snprintf(star_string, star_string_len, "Melee Stars: %.1f", melee);
	add_submenu_option(submenu, star_string, editor_switch_submenu);

	free(star_string);

	for (int i = 0; i < 6 && mods[i] != NULL; i++) {
		add_submenu_option(submenu, mods[i], editor_switch_submenu);
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

		kill_children_reset_page(old, load_save);

		context->save_loaded = false;
	}
	// Load the menu context regarding the saves, and load the save
	context->file_index = selected->prev->selected;
	savefile_init(&context->save, context->files[context->file_index]);

	build_weapon_menus(selected, context->save);

	selected->action = editor_switch_submenu;
	context->save_loaded = true;
}


void load_weapons(void *menu_ptr, void *context_ptr)
{
	Menu *menu = (Menu*)menu_ptr;
	MenuContext *context = (MenuContext*)context_ptr;
	Submenu *selected = menu->curr;

	build_mod_menus(selected, context->save);
	selected->action = editor_switch_submenu;
}
