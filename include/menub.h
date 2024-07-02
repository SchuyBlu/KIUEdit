/*
 * Author: Schuy
 * Module meant for constructing and navigating the menu specific to KIU.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "menu.h"
#include "file.h"


/*
 * Initializes the save for kid icarus uprising, and returns a Menu.
 */
Menu save_menu_init(void);


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

