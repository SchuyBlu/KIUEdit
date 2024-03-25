/*
 * Author: Schuy
 * Meant to be used as a pagination API, and has been written to be as
 * standalone as possible.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#define BIT(n) (1U<<(n))


/*
 * `Action` is a type of function that is intended to be something of a
 * polymorphic function. There are, however, some rules to its intended
 * use, specifically in its parameters.
 * `pages` - pointer to a Pages struct, defined below. Needs to be cast.
 * `data` - Pointer to data. Can be anything, but needs to be correctly
 * cast by the function.
 * Casting is responsibility of function.
 */
typedef void (*Action)(void *pages, void *data);


/*
 * Stores all the information that one might need to use a pagination system.
 * `desc` - string describing page.
 * `len` - length of options array.
 * `cap` - capacity of options array.
 * `selected` - currently selected option in options array.
 * `prev` - the previous Page struct that was accessed.
 * `options` - array of Page structs that can be moved to.
 * `action` - pointer to function to be executed upon an action.
 * `data` - pointer to data that is possibly needed for action function.
 * `dealloc_idx` - index of branch being deallocated.
 */
struct Page {
	char *desc;
	uint32_t len;
	uint32_t cap;
	uint32_t selected;
	struct Page *prev;
	struct Page **options;
	Action action;
	void *data;
	uint32_t dealloc_idx;
};


/*
 * Handler for each page.
 * title - Title of system using the paginator. Will be displayed at all times.
 * `root` - root page of paginator.
 * `curr` - current page of paginator.
 */
struct Pages {
	char *title;
	struct Page *root;
	struct Page *curr;
};


/*
 * Initializes a page passed to the function.
 * `page` - page being initialized.
 * `desc` - page description.
 * `action` - Pointer to an Action function.
 */
void page_init(struct Page *page, char *desc, Action action);


/*
 * Initializes the title, and NULL initializes the first page.
 * `pages` - Pages struct to be initialized.
 * `title` - title to be used for the Pages struct.
 * `action` - Action to apply to the first page.
 */
void pages_init(struct Pages *pages, char *title, Action action);


/*
 * Internal helper for destroy_pages defined below this function. Allows
 * The function to be called recursively.
 * `page` - page being destroyed.
 */
void _destroy_pages(struct Page *page);


/*
 * Destroys all allocated members of the Pages struct.
 * `pages` - Pages struct to be destroyed.
 */
void destroy_pages(struct Pages *pages);


/*
 * Sets the description of a page.
 * `page` - pointer to page being modified.
 * `desc` - new description for page.
 */
void set_page_desc(struct Page *page, char *desc);


/*
 * Adds an option to the page being specified.
 * `page` - pointer to page being modified.
 * `desc` - description for new page.
 * `action` - function defining the action for the page.
 * Returns a pointer to the new page.
 */
struct Page *add_page_option(struct Page *page, char *desc, Action action);


/*
 * Prints a page out. If page is set to `NULL`, it will print the page that is
 * currently set to curr in the pages struct.
 * `pages` - pages struct holding all pages.
 * `page` - specific page to be printed.
 */
void print_page(struct Pages *pages, struct Page *page);


/*
 * Switches the page depending on the input given in `k_down`. If `k_down` is
 * 0, it switches to the selected page, if `k_down` is 1 it switches to the
 * previous page.
 * NOTE: While in the function definition the key being pressed down is vague,
 * it is meant for the 3ds.h library in production.
 * `pages` - pages being processed.
 * `k_down` - key being pressed down.
 */
void switch_page(void *pages_ptr, void *k_down_ptr);


/*
 * Moves the cursor to the next option depending on the input given. Will
 * wrap around if the cursor moves beyond the length of the options list.
 * `pages` - pages being processed.
 * `k_down` - key being pressed down.
 */
void move_page_cursor(struct Pages *pages, uint32_t k_down);

