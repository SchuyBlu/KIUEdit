// MIT License
//
// Copyright (c) 2023 Schuy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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
