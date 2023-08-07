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
#include "unity.h"


struct Pages pages;

void setUp(void)
{
        pages_init(&pages, "Testing Paginator", MISC_PAGE);
}


void tearDown(void)
{
        destroy_pages(&pages);
}


void test_givenPagesAreInitialized_should_displayCorrectTitle(void)
{
        TEST_ASSERT_EQUAL_STRING("Testing Paginator", pages.title);
}


void test_givenNewDescriptionAdded_should_correctlyChangeDescription(void)
{
        set_page_desc(pages.curr, "Example of page description.");
        TEST_ASSERT_EQUAL_STRING("Example of page description.", pages.curr->desc);
}


void test_givenNewOptionAdded_should_correctlyAddNewOption(void)
{
        add_page_option(pages.curr, "New option.", MISC_PAGE);
        TEST_ASSERT_EQUAL_INT(MISC_PAGE, pages.curr->options[pages.curr->len - 1]->id);
        TEST_ASSERT_EQUAL_STRING("New option.", pages.curr->options[pages.curr->len - 1]->desc);
        TEST_ASSERT_EQUAL_UINT32(1, pages.curr->len);
        TEST_ASSERT_EQUAL_UINT32(1, pages.curr->cap);
        TEST_ASSERT_EQUAL_UINT32(0, pages.curr->options[pages.curr->len - 1]->len);
        TEST_ASSERT_EQUAL_UINT32(1, pages.curr->options[pages.curr->len - 1]->cap);
        TEST_ASSERT_EQUAL_UINT32(0, pages.curr->options[pages.curr->len - 1]->selected);
        TEST_ASSERT_EQUAL_UINT32(0, pages.curr->options[pages.curr->len - 1]->dealloc_idx);
        TEST_ASSERT_EQUAL_UINT64(pages.curr, pages.curr->options[pages.curr->len - 1]->prev);
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_givenPagesAreInitialized_should_displayCorrectTitle);
        RUN_TEST(test_givenNewDescriptionAdded_should_correctlyChangeDescription);
        RUN_TEST(test_givenNewOptionAdded_should_correctlyAddNewOption);
        return UNITY_END();
}

