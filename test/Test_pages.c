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


void test_givenAThirdLevelPage_should_beConstructedCorrectly(void)
{
        // This test also exists to see whether or not memory for pages that
        // are multiple layers deep are being freed correctly, but testing
        // for this is being handled by the address sanitizer.
        struct Page *page, *new_page;
        struct Page *prev_page;

        // We know this works because it's tested in the last test.
        add_page_option(pages.curr, "New option.", MISC_PAGE);

        page = pages.curr->options[pages.curr->len - 1];
        add_page_option(page, "Third level page.", MISC_PAGE);

        new_page = page->options[page->len - 1];
        TEST_ASSERT_EQUAL_INT(MISC_PAGE, new_page->id);
        TEST_ASSERT_EQUAL_STRING("Third level page.", new_page->desc);
        TEST_ASSERT_EQUAL_UINT32(0, new_page->len);
        TEST_ASSERT_EQUAL_UINT32(1, new_page->cap);
        TEST_ASSERT_EQUAL_UINT32(0, new_page->selected);
        TEST_ASSERT_EQUAL_UINT32(0, new_page->dealloc_idx);
        TEST_ASSERT_EQUAL_UINT64(new_page->prev, page);


        // Now check whether the current page is accessible from the
        // current page.
        TEST_ASSERT_EQUAL_INT(MISC_PAGE, page->options[page->len - 1]->id);
        TEST_ASSERT_EQUAL_STRING("Third level page.", page->options[page->len - 1]->desc);
        TEST_ASSERT_EQUAL_UINT32(0, page->options[page->len - 1]->len);
        TEST_ASSERT_EQUAL_UINT32(1, page->options[page->len - 1]->cap);
        TEST_ASSERT_EQUAL_UINT32(0, page->options[page->len - 1]->selected);
        TEST_ASSERT_EQUAL_UINT32(0, page->options[page->len - 1]->dealloc_idx);
}


void test_givenThreePagesWithTwoLevelsEach_should_correctContainAllInformation(void)
{
        struct Page *root;
        struct Page *root_opt1, *root_opt2, *root_opt3;
        struct Page *root_opt1_opt1, *root_opt1_opt2;
        struct Page *root_opt2_opt1, *root_opt2_opt2;
        struct Page *root_opt3_opt1, *root_opt3_opt2;

        // For anyone struggling to follow, this is the tree we're making:
        //
        // root
        //  |
        //  ----> root_opt1
        //  |         |
        //  |         -------> root_opt1_opt1
        //  |         |
        //  |         -------> root_opt1_opt2
        //  |
        //  ----> root_opt2
        //  |         |
        //  |         -------> root_opt2_opt1
        //  |         |
        //  |         -------> root_opt2_opt2
        //  |
        //  ----> root_opt3
        //            |
        //            -------> root_opt3_opt1
        //            |
        //            -------> root_opt3_opt2
        //
        // We're testing to see that every branch functions correctly.
        // We're also testing to see if the recursive free function
        // can properly handle freeing memory for several multilevel
        // pages.
        
        root = pages.curr;
        add_page_option(root, "Root -> Option 1", MISC_PAGE);
        add_page_option(root, "Root -> Option 2", MISC_PAGE);
        add_page_option(root, "Root -> Option 3", MISC_PAGE);

        // Assign root options
        root_opt1 = root->options[0];
        root_opt2 = root->options[1];
        root_opt3 = root->options[root->len - 1]; // Just making sure length is working correctly

        // Test all root options. Really only testing to make sure each branch
        // is entirely unique.
        TEST_ASSERT_EQUAL_UINT32(3, root->len);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 1", root_opt1->desc);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 2", root_opt2->desc);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 3", root_opt3->desc);

        // Create outer branches for root_opt1 and test.
        add_page_option(root_opt1, "Root -> Option 1 -> Option 1", MISC_PAGE);
        add_page_option(root_opt1, "Root -> Option 1 -> Option 2", MISC_PAGE);
        root_opt1_opt1 = root_opt1->options[0];
        root_opt1_opt2 = root_opt1->options[1];
        TEST_ASSERT_EQUAL_UINT32(2, root_opt1->len);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 1 -> Option 1", root_opt1_opt1->desc);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 1 -> Option 2", root_opt1_opt2->desc);

        // Create outer branches for root_opt2 and test.
        add_page_option(root_opt2, "Root -> Option 2 -> Option 1", MISC_PAGE);
        add_page_option(root_opt2, "Root -> Option 2 -> Option 2", MISC_PAGE);
        root_opt2_opt1 = root_opt2->options[0];
        root_opt2_opt2 = root_opt2->options[1];
        TEST_ASSERT_EQUAL_UINT32(2, root_opt2->len);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 2 -> Option 1", root_opt2_opt1->desc);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 2 -> Option 2", root_opt2_opt2->desc);

        // Create outer branches for root_opt3 and test.
        add_page_option(root_opt3, "Root -> Option 3 -> Option 1", MISC_PAGE);
        add_page_option(root_opt3, "Root -> Option 3 -> Option 2", MISC_PAGE);
        root_opt3_opt1 = root_opt3->options[0];
        root_opt3_opt2 = root_opt3->options[1];
        TEST_ASSERT_EQUAL_UINT32(2, root_opt3->len);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 3 -> Option 1", root_opt3_opt1->desc);
        TEST_ASSERT_EQUAL_STRING("Root -> Option 3 -> Option 2", root_opt3_opt2->desc);
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_givenPagesAreInitialized_should_displayCorrectTitle);
        RUN_TEST(test_givenNewDescriptionAdded_should_correctlyChangeDescription);
        RUN_TEST(test_givenNewOptionAdded_should_correctlyAddNewOption);
        RUN_TEST(test_givenAThirdLevelPage_should_beConstructedCorrectly);
        RUN_TEST(test_givenThreePagesWithTwoLevelsEach_should_correctContainAllInformation);
        return UNITY_END();
}
