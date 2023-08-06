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


void pages_init(struct Pages *pages, char *title)
{
        // Allocate for the title and copy over the title.
        pages->title = malloc(sizeof(char) * strlen(title) + 1);
        assert(pages->title);
        strcpy(pages->title, title);

        // Allocate first page and set as root.
        pages->curr = malloc(sizeof(struct Page));
        assert(pages->curr);
        pages->root = pages->curr;

        // NULL initialize first page.
        pages->curr->len = 0;
        pages->curr->selected = 0;
        pages->curr->dealloc_idx = 0;
        pages->curr->prev = NULL;
        pages->curr->options = NULL;
        pages->curr->load = NULL;
}


void _destroy_pages(struct Page *page)
{
        for (; page->dealloc_idx < page->len; page->dealloc_idx++) {
                _destroy_pages(&page->options[page->dealloc_idx]);
        }

        if (page->options) {
                free(page->options);
        }
}


void destroy_pages(struct Pages *pages)
{
        _destroy_pages(pages->root);
        free(pages->root);
        free(pages->title);
}

