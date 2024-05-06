/*
 * Author: Schuy
 * Meant to be used as a menu API, and has been written to be as
 * standalone as possible.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#define BIT(n) (1U<<(n))


/*
 * `event` is a type of function that is intended to be something of a
 * polymorphic function. There are, however, some rules to its intended
 * use, specifically in its parameters.
 * `menu` - pointer to a Submenu struct, defined below. Needs to be cast.
 * `data` - Pointer to data. Can be anything, but needs to be correctly
 * cast by the function.
 * Casting is responsibility of function.
 */
typedef void (*event)(void *menu, void *data);


/*
 * Stores all the information that one might need to use menu system.
 * `desc` - string describing the submenu.
 * `len` - length of options array.
 * `cap` - capacity of options array.
 * `selected` - currently selected option in options array.
 * `prev` - the previous Submenu struct that was accessed.
 * `options` - array of Submenu structs that can be moved to.
 * `action` - pointer to function to be executed upon an action.
 * `data` - pointer to data that is possibly needed for action function.
 * `dealloc_idx` - index of branch being deallocated.
 */
typedef struct Submenu {
	char *desc;
	uint32_t len;
	uint32_t cap;
	uint32_t selected;
	struct Submenu *prev;
	struct Submenu **options;
	event action;
	void *data;
	uint32_t dealloc_idx;
} Submenu;


/*
 * Handler for each submenu.
 * title - Title of menu system. Will be displayed at all times.
 * `root` - root submenu of menu system.
 * `curr` - current submenu of menu system.
 */
typedef struct Menu {
	char *title;
	struct Submenu *root;
	struct Submenu *curr;
} Menu;


/*
 * Initializes a submenu passed to the function.
 * `submenu` - submenu being initialized.
 * `desc` - page description.
 * `action` - Pointer to an Action function.
 */
void submenu_init(Submenu *submenu, char *desc, event action);


/*
 * Initializes the title, and NULL initializes the root submenu.
 * `menu` - Menu struct to be initialized.
 * `title` - title to be used for the menu system.
 * `action` - Action to apply to the root submenu.
 */
void menu_init(Menu *menu, char *title, event action);


/*
 * Internal helper for destroy_menu defined below this function. Allows
 * The function to be called recursively.
 * `submenu` - Internal submenu being destroyed.
 */
static void _destroy_menu(Submenu *submenu);


/*
 * Destroys all allocated members of the Menu struct.
 * `menu` - Menu struct to be destroyed.
 */
void destroy_menu(Menu *menu);


/*
 * Sets the description of a submenu.
 * `submenu` - pointer to submenu being modified.
 * `desc` - new description for the submenu.
 */
void set_submenu_desc(Submenu *submenu, char *desc);


/*
 * Adds an option to the submenu being specified.
 * `submenu` - pointer to submenu being modified.
 * `desc` - description for new submenu.
 * `action` - function defining the action for the submenu.
 * Returns a pointer to the new submenu.
 */
Submenu *add_submenu_option(Submenu *submenu, char *desc, event action);


/*
 * Prints menu.
 * `submenu` - specific submenu to be printed.
 */
void print_menu(Menu *menu, Submenu *submenu);


/*
 * Switches the submenu depending on the input given in `k_down`. If 
 * `k_down` is 0, it switches to the selected submenu, if `k_down` is 1
 * it switches to the previous submenu.
 * NOTE: While in the function definition the key being pressed down 
 * is vague, it is meant for the 3ds.h library in production.
 * `menu_ptr` - Menu being processed.
 * `k_down_ptr` - key being pressed down.
 */
void switch_submenu(void *menu_ptr, void *k_down_ptr);


/*
 * Moves the cursor to the next option depending on the input given. Will
 * wrap around if the cursor moves beyond the length of the options list.
 * `menu` - pages being processed.
 * `k_down` - key being pressed down.
 */
void move_menu_cursor(Menu *menu, uint32_t k_down);

