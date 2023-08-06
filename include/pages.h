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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/*
 * Page
 * ----
 * Stores all the information that one might need to use a pagination system.
 * Attrs:
 * len - length of options array.
 * selected - currently selected option in options array.
 * dealloc_idx - index of branch being deallocated.
 * prev - the previous Page struct that was accessed.
 * options - array of Page structs that can be moved to.
 * load - pointer to function to be executed when page is created.
 */
struct Page {
        uint32_t len;
        uint32_t selected;
        uint32_t dealloc_idx;
        struct Page *prev;
        struct Page *options;
        void *load;
};


/*
 * Pages
 * -----
 * Handler for each page.
 * Attrs:
 * title - Title of system using the paginator. Will be displayed at all times.
 * root - root page of paginator.
 * curr - current page of paginator.
 */
struct Pages {
        char *title;
        struct Page *root;
        struct Page *curr;
};


/*
 * pages_init
 * ----------
 * Initializes the title, and NULL initializes the first page.
 * Params:
 * pages - Pages struct to be initialized.
 * title - title to be used for the Pages struct.
 */
void pages_init(struct Pages *pages, char *title);


/*
 * _destroy_pages
 * --------------
 * Internal helper for destroy_pages defined below this function. Allows
 * The function to be called recursively.
 * Params:
 * page - page being destroyed.
 */
void _destroy_pages(struct Page *page);


/*
 * destroy_pages
 * -------------
 * Destroys all allocated members of the Pages struct.
 * Params:
 * pages - Pages struct to be destroyed.
 */
void destroy_pages(struct Pages *pages);

