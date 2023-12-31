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
 * IDs that represent page types. Can be modified as needed depending on
 * the application.
 */
typedef enum {
        MISC_PAGE,
        SAVE_PAGE,
        NULL_PAGE
} P_TYPE;

/*
 * Stores all the information that one might need to use a pagination system.
 * `id` - id describing page.
 * `desc` - string describing page.
 * `len` - length of options array.
 * `cap` - capacity of options array.
 * `selected` - currently selected option in options array.
 * `prev` - the previous Page struct that was accessed.
 * `options` - array of Page structs that can be moved to.
 * `load` - pointer to function to be executed when page is created.
 * `dealloc_idx` - index of branch being deallocated.
 */
struct Page {
        P_TYPE id;
        char *desc;
        uint32_t len;
        uint32_t cap;
        uint32_t selected;
        struct Page *prev;
        struct Page **options;
        void *load;
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
 * `desc` - page description.
 * `id` - type of page.
 */
void page_init(struct Page *page, char *desc, P_TYPE id);


/*
 * Initializes the title, and NULL initializes the first page.
 * `pages` - Pages struct to be initialized.
 * `title` - title to be used for the Pages struct.
 * id - type of page.
 */
void pages_init(struct Pages *pages, char *title, P_TYPE id);


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
 * `id` - type of page being added.
 */
void add_page_option(struct Page *page, char *desc, P_TYPE id);


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
void switch_page(struct Pages *pages, uint32_t k_down);


/*
 * Moves the cursor to the next option depending on the input given. Will
 * wrap around if the cursor moves beyond the length of the options list.
 * `pages` - pages being processed.
 * `k_down` - key being pressed down.
 */
void move_page_cursor(struct Pages *pages, uint32_t k_down);

