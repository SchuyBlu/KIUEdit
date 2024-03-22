#include "pages.h"


void page_init(struct Page *page, char *desc, P_TYPE id)
{
	if (!desc) {
		page->desc = NULL;
	} else {
		page->desc = malloc(sizeof(char) * strlen(desc) + 1);
		assert(page->desc);
		strcpy(page->desc, desc);
	}

	page->id = id;
	page->len = 0;
	page->cap = 1;
	page->selected = 0;
	page->dealloc_idx = 0;
	page->prev = NULL;
	page->options = NULL;
	page->load = NULL;
}


void pages_init(struct Pages *pages, char *title, P_TYPE id)
{
	// Allocate for the title and copy over the title.
	pages->title = malloc(sizeof(char) * strlen(title) + 1);
	assert(pages->title);
	strcpy(pages->title, title);

	// Allocate first page and set as root.
	pages->curr = malloc(sizeof(struct Page));
	assert(pages->curr);
	pages->root = pages->curr;

	page_init(pages->curr, NULL, id);
}


void _destroy_pages(struct Page *page)
{
	for (; page->dealloc_idx < page->len; page->dealloc_idx++) {
		_destroy_pages(page->options[page->dealloc_idx]);
	}

	if (page->options) {
		free(page->options);
	}

	if (page->desc) {
		free(page->desc);
	}
	free(page);
}


void destroy_pages(struct Pages *pages)
{
	_destroy_pages(pages->root);
	free(pages->title);
}


void set_page_desc(struct Page *page, char *desc)
{
	// Free the page description if it was set in the past.
	if (page->desc) {
		free(page->desc);
	}

	page->desc = malloc(sizeof(char) * strlen(desc) + 1);
	assert(page->desc);

	strcpy(page->desc, desc);
}


void add_page_option(struct Page *page, char *desc, P_TYPE id)
{
	struct Page *new_page;

	// Allocate for array
	if (page->len == 0) {
		page->options = malloc(sizeof(struct Page *) * page->cap);
		assert(page->options);
	} else if (page->len == page->cap) {
		page->cap *= 2;
		page->options = realloc(page->options, sizeof(struct Page *) * page->cap);
		assert(page->options);
	}

	new_page = malloc(sizeof(struct Page));
	assert(new_page);
	page_init(new_page, desc, id);
	new_page->prev = page;

	page->options[page->len] = new_page;
	page->len++;
}


void print_page(struct Pages *pages, struct Page *page)
{
	struct Page *page_to_print;

	// Handle whether or not page is NULL.
	if (!page) {
		page_to_print = pages->curr;
	} else {
		page_to_print = page;
	}

	printf("%s\n", pages->title);
	for (int i = 0; i < strlen(pages->title); i++) {
		putchar('_');
	}
	putchar('\n');

	printf("%s\n", page_to_print->desc);
	for (int i = 0; i < page_to_print->len; i++) {
		if (i == page_to_print->selected) {
			printf("> %s\n", page_to_print->options[i]->desc);
		} else {
			printf("  %s\n", page_to_print->options[i]->desc);
		}
	}
}


void switch_page(struct Pages *pages, uint32_t k_down)
{
	if (k_down & BIT(1) && pages->curr->prev) {
		pages->curr = pages->curr->prev;
		return;
	}

	if (k_down & BIT(0) && pages->curr->len > 0) {
		pages->curr = pages->curr->options[pages->curr->selected];
	}
}


void move_page_cursor(struct Pages *pages, uint32_t k_down)
{
	if (k_down & BIT(7)) {
		pages->curr->selected++;
		if (pages->curr->selected >= pages->curr->len) {
			pages->curr->selected = 0;
		}
	}

	if (k_down & BIT(6)) {
		pages->curr->selected--;
		if (pages->curr->selected < 0) {
			pages->curr->selected = pages->curr->len - 1;
		}
	}
}

