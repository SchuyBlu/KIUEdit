#include "menu.h"


void submenu_init(Submenu *submenu, const char *desc, event action)
{
	if (!desc) {
		submenu->desc = NULL;
	} else {
		submenu->desc = malloc(sizeof(char) * strlen(desc) + 1);
		assert(submenu->desc);
		strcpy(submenu->desc, desc);
	}

	submenu->len = 0;
	submenu->cap = 1;
	submenu->selected = 0;
	submenu->dealloc_idx = 0;
	submenu->view_top = 0;
	submenu->prev = NULL;
	submenu->options = NULL;
	submenu->action = action;
	submenu->data = NULL;
}


void menu_init(Menu *menu, char *title, event action)
{
	// Allocate for the title and copy over the title.
	menu->title = malloc(sizeof(char) * strlen(title) + 1);
	assert(menu->title);
	strcpy(menu->title, title);

	// Allocate first page and set as root.
	menu->curr = malloc(sizeof(Submenu));
	assert(menu->curr);
	menu->root = menu->curr;

	submenu_init(menu->curr, NULL, action);
}


void _destroy_menu(Submenu *submenu)
{
	for (; submenu->dealloc_idx < submenu->len; submenu->dealloc_idx++) {
		_destroy_menu(submenu->options[submenu->dealloc_idx]);
	}

	if (submenu->options) {
		free(submenu->options);
	}

	if (submenu->desc) {
		free(submenu->desc);
	}
	free(submenu);
}


void destroy_menu(Menu *menu)
{
	_destroy_menu(menu->root);
	free(menu->title);
}


void set_submenu_desc(Submenu *submenu, char *desc)
{
	// Free the page description if it was set in the past.
	if (submenu->desc) {
		free(submenu->desc);
	}

	submenu->desc = malloc(sizeof(char) * strlen(desc) + 1);
	assert(submenu->desc);

	strcpy(submenu->desc, desc);
}


char *get_submenu_desc(Submenu *submenu)
{
	return submenu->desc;
}


Submenu *add_submenu_option(Submenu *submenu, const char *desc, event action)
{
	Submenu *new_submenu;

	// Allocate for array
	if (submenu->len == 0) {
		submenu->options = malloc(sizeof(Submenu*) * submenu->cap);
		assert(submenu->options);
	} else if (submenu->len == submenu->cap) {
		submenu->cap *= 2;
		submenu->options = realloc(submenu->options, sizeof(Submenu*) * submenu->cap);
		assert(submenu->options);
	}

	new_submenu = malloc(sizeof(Submenu));
	assert(new_submenu);
	submenu_init(new_submenu, desc, action);
	new_submenu->prev = submenu;

	submenu->options[submenu->len] = new_submenu;
	submenu->len++;

	return new_submenu;
}


/*
void print_menu(struct Menu *menu, Submenu *submenu)
{
	Submenu *submenu_to_print;

	// Handle whether or not page is NULL.
	if (!submenu) {
		submenu_to_print = menu->curr;
	} else {
		submenu_to_print = submenu;
	}

	printf("%s\n", menu->title);
	for (int i = 0; i < strlen(menu->title); i++) {
		putchar('_');
	}
	printf("\n\n");

	printf("%s\n", submenu_to_print->desc);
	for (int i = 0; i < submenu_to_print->len; i++) {
		if (i == submenu_to_print->selected) {
			printf("> %s\n", submenu_to_print->options[i]->desc);
		} else {
			printf("  %s\n", submenu_to_print->options[i]->desc);
		}
	}
}
*/
void print_menu(struct Menu *menu, Submenu *submenu)
{
    Submenu *page = NULL;
	int top = 0, end = 0;

	if (submenu) {
		page = submenu;
	} else {
		page = menu->curr;
	}

    printf("%s\n", menu->title);
    for (int i = 0; i < (int)strlen(menu->title); i++) {
        putchar('_');
    }
    printf("\n\n");

    printf("%s\n", page->desc);

    if (page->len == 0)
		return;

    top = page->view_top;
    end = top + VISIBLE_ROWS;
    if (end > page->len) end = page->len;

    for (uint32_t i = top; i < end; ++i) {
        if (i == page->selected) {
            printf("> %s\n", page->options[i]->desc);
        } else {
            printf("  %s\n", page->options[i]->desc);
        }
    }
}



void switch_submenu(void *menu_ptr, void *k_down_ptr)
{
	Menu *menu = (Menu*)menu_ptr;
	uint32_t k_down = *((uint32_t*)k_down_ptr);

	if (k_down & BIT(1) && menu->curr->prev) {
		menu->curr = menu->curr->prev;
		return;
	}

	// Comment for testing
	if (k_down & BIT(0) && menu->curr->len > 0) {
		menu->curr = menu->curr->options[menu->curr->selected];
	}
}


/*
void move_menu_cursor(Menu *menu, uint32_t k_down)
{
    Submenu *page = menu->curr;
    if (!page || page->len == 0) return;

    // DOWN
    if (k_down & BIT(7)) {
        // wrap-around increment (safe because len > 0)
        page->selected = (page->selected + 1) % page->len;

        // If cursor fell below the window, scroll down
        if (page->selected >= page->view_top + VISIBLE_ROWS) {
            page->view_top = page->selected - (VISIBLE_ROWS - 1);
        }
    }

    // UP
    if (k_down & BIT(6)) {
        if (page->selected == 0) {
			page->selected = page->len - 1;
		} else {
			;page->selected--;
		}

        // If cursor moved above the window, scroll up
        if (page->selected < page->view_top) {
            page->view_top = page->selected;
        }
    }
}
*/

void move_menu_cursor(Menu *menu, uint32_t k_down)
{
    Submenu *page = menu->curr;
    uint32_t prev = 0;
    uint32_t n = 0;
    uint32_t win = VISIBLE_ROWS;
	uint32_t target_top = 0;
    uint32_t last_top = 0;

    if (!page || page->len == 0)
		return;

    prev = page->selected;
    n = page->len;
	target_top = 0;

	// If there are more than VISIBLE_ROWS rows, the last window starts at
	// page->len - VISIBLE_ROWS. Otherwise, starts at 0.
    last_top = (n > win) ? (n - win) : 0;

    // DOWN
    if (k_down & BIT(7)) {
        page->selected = (page->selected + 1) % n;

        if (prev == n - 1 && page->selected == 0) {
            page->view_top = 0;
        } else if (page->selected >= page->view_top + win) {
            target_top = page->selected - (win - 1);
			// Clamp view top so that it never goes beyond last top. Prevents scrolling
			// past page->len - VISIBLE_ROWS.
            page->view_top = (target_top > last_top) ? last_top : target_top;
        }
    }

    // UP
    if (k_down & BIT(6)) {
        if (page->selected == 0) {
			page->selected = n - 1;
		} else {
			;page->selected--;
		}

        if (prev == 0 && page->selected == n - 1) {
            page->view_top = last_top;
        } else if (page->selected < page->view_top) {
            page->view_top = page->selected;
        }
    }
}

