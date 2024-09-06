/*
 * Author: Schuy
 * Module meant for constructing and navigating the menu specific to KIU.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include "menu.h"
#include "file.h"


/*
 * Contains miscellaneous data required for the editor to run the menu.
 * `save` - Save file containing KIU save data.
 * `save_loaded` - boolean to determine wether or not the save has loaded.
 * `key_down` - Contains last key press.
 */
typedef struct MenuContext {
    SaveFile save;
    bool save_loaded;
    uint32_t key_down;
    char **files;
    uint32_t files_len;
    uint32_t file_index;
} MenuContext;


/*
 * Contains a menu as a navigation interface for the user, and a savefile for
 * storing the data.
 * `save` - Save file containing KIU save info.
 * `menu` - Menu to interface with.
 * `context` - Data required to run menu in loop.
 */
typedef struct Editor {
    Menu menu;
    MenuContext ctx;
} Editor;


/*
 * Initializes the save for kid icarus uprising, and returns an editor.
 */
Editor save_menu_init(void);


/*
 * Simple string comparator function to be called in `get_save_strings`.
 */
int comp_string(const void *string1, const void *string2);


/*
 * Retrieves the list of saves. Should only contain 01.SAV, 02.SAV, 03.SAV.
 * `name` - directory path to be opened.
 * `len` - Will contain the length of the array after return.
 */
char **get_save_strings(const char *dir_path, int *len);


/*
 * Adds each weapon in the save to the menu.
 * `submenu` - submenu being added to.
 * `save` - save file being read from.
 */
void build_weapon_menus(Submenu *submenu, SaveFile save);


/*
 * Function to be attached as an event to a submenu.
 * `menu_ptr` - Pointer to the mneu.
 * `context_ptr` - Pointer the context for menu.
 */
void load_save(void *menu_ptr, void *context_ptr);
