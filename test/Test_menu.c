// NOTE: Please note that the menu itself must be tested manually once build to assure
// all the pieces are assembled in the correct order.
#include "menu.h"
#include "unity.h"


Menu menu;


void setUp(void)
{
	menu_init(&menu, "Testing Menu", switch_submenu);
	set_submenu_desc(menu.curr, "This is a menu!");
}


void tearDown(void)
{
	destroy_menu(&menu);
}


void test_givenMenuIsInitialized_should_displayCorrectTitle(void)
{
	TEST_ASSERT_EQUAL_STRING("Testing Menu", menu.title);
}


void test_givenNewDescriptionAdded_should_correctlyChangeDescription(void)
{
	set_submenu_desc(menu.curr, "Example of submenu description.");
	TEST_ASSERT_EQUAL_STRING("Example of submenu description.", menu.curr->desc);
}


void test_givenGetDescription_should_correctlyReturnDescription(void)
{
	char *received;

	set_submenu_desc(menu.curr, "Example of submenu description.");
	received = get_submenu_desc(menu.curr);
	TEST_ASSERT_EQUAL_STRING("Example of submenu description.", received);
}


void test_givenAThirdLevelPage_should_beConstructedCorrectly(void)
{
	// This test also exists to see whether or not memory for submenus
	// that are multiple layers deep are being freed correctly, but 
	// testing for this is being handled by the address sanitizer.
	Submenu *submenu, *new_submenu;

	// We know this works because it's tested in the last test.
	add_submenu_option(menu.curr, "New option.", switch_submenu);

	submenu = menu.curr->options[menu.curr->len - 1];
	add_submenu_option(submenu, "Third level menu.", switch_submenu);

	new_submenu = submenu->options[submenu->len - 1];
	TEST_ASSERT_EQUAL_STRING("Third level menu.", new_submenu->desc);
	TEST_ASSERT_EQUAL_UINT32(0, new_submenu->len);
	TEST_ASSERT_EQUAL_UINT32(1, new_submenu->cap);
	TEST_ASSERT_EQUAL_UINT32(0, new_submenu->selected);
	TEST_ASSERT_EQUAL_UINT32(0, new_submenu->dealloc_idx);
	TEST_ASSERT_EQUAL_UINT64(new_submenu->prev, submenu);


	// Now check whether the current submenu is accessible from the
	// current submenu.
	TEST_ASSERT_EQUAL_STRING("Third level menu.", submenu->options[submenu->len - 1]->desc);
	TEST_ASSERT_EQUAL_UINT32(0, submenu->options[submenu->len - 1]->len);
	TEST_ASSERT_EQUAL_UINT32(1, submenu->options[submenu->len - 1]->cap);
	TEST_ASSERT_EQUAL_UINT32(0, submenu->options[submenu->len - 1]->selected);
	TEST_ASSERT_EQUAL_UINT32(0, submenu->options[submenu->len - 1]->dealloc_idx);
}


void test_givenThreeSubmenusWithTwoLevelsEach_should_correctContainAllInformation(void)
{
	Submenu *root;
	Submenu *root_opt1, *root_opt2, *root_opt3;
	Submenu *root_opt1_opt1, *root_opt1_opt2;
	Submenu *root_opt2_opt1, *root_opt2_opt2;
	Submenu *root_opt3_opt1, *root_opt3_opt2;

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
	
	root = menu.curr;
	add_submenu_option(root, "Root -> Option 1", switch_submenu);
	add_submenu_option(root, "Root -> Option 2", switch_submenu);
	add_submenu_option(root, "Root -> Option 3", switch_submenu);

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
	add_submenu_option(root_opt1, "Root -> Option 1 -> Option 1", switch_submenu);
	add_submenu_option(root_opt1, "Root -> Option 1 -> Option 2", switch_submenu);
	root_opt1_opt1 = root_opt1->options[0];
	root_opt1_opt2 = root_opt1->options[1];
	TEST_ASSERT_EQUAL_UINT32(2, root_opt1->len);
	TEST_ASSERT_EQUAL_STRING("Root -> Option 1 -> Option 1", root_opt1_opt1->desc);
	TEST_ASSERT_EQUAL_STRING("Root -> Option 1 -> Option 2", root_opt1_opt2->desc);

	// Create outer branches for root_opt2 and test.
	add_submenu_option(root_opt2, "Root -> Option 2 -> Option 1", switch_submenu);
	add_submenu_option(root_opt2, "Root -> Option 2 -> Option 2", switch_submenu);
	root_opt2_opt1 = root_opt2->options[0];
	root_opt2_opt2 = root_opt2->options[1];
	TEST_ASSERT_EQUAL_UINT32(2, root_opt2->len);
	TEST_ASSERT_EQUAL_STRING("Root -> Option 2 -> Option 1", root_opt2_opt1->desc);
	TEST_ASSERT_EQUAL_STRING("Root -> Option 2 -> Option 2", root_opt2_opt2->desc);

	// Create outer branches for root_opt3 and test.
	add_submenu_option(root_opt3, "Root -> Option 3 -> Option 1", switch_submenu);
	add_submenu_option(root_opt3, "Root -> Option 3 -> Option 2", switch_submenu);
	root_opt3_opt1 = root_opt3->options[0];
	root_opt3_opt2 = root_opt3->options[1];
	TEST_ASSERT_EQUAL_UINT32(2, root_opt3->len);
	TEST_ASSERT_EQUAL_STRING("Root -> Option 3 -> Option 1", root_opt3_opt1->desc);
	TEST_ASSERT_EQUAL_STRING("Root -> Option 3 -> Option 2", root_opt3_opt2->desc);
}


void test_ifPrintIsCalled_should_displayCorrectly(void)
{
	add_submenu_option(menu.curr, "Option 1", switch_submenu);
	add_submenu_option(menu.curr, "Option 2", switch_submenu);
	add_submenu_option(menu.curr, "Option 3", switch_submenu);
	add_submenu_option(menu.curr, "Option 4", switch_submenu);
	print_menu(&menu, NULL);
}


void test_givenSubmenuIsChanged_then_newCurrentShouldBeUpdated(void)
{
	uint32_t a = BIT(0), b = BIT(1);

	add_submenu_option(menu.curr, "Option 1", switch_submenu);
	add_submenu_option(menu.curr, "Option 2", switch_submenu);

	// First try to flip back. Nothing should happen and it should reamin
	// on the root submenu.
	switch_submenu(&menu, &b);
	TEST_ASSERT_EQUAL_STRING("This is a menu!", menu.curr->desc);

	// Now test flipping forward.
	switch_submenu(&menu, &a);
	TEST_ASSERT_EQUAL_STRING("Option 1", menu.curr->desc);

	// Now test flipping forward again. Nothing should happen and it should
	// remain on the Option 1 page.
	switch_submenu(&menu, &a);
	TEST_ASSERT_EQUAL_STRING("Option 1", menu.curr->desc);

	// Now test flipping back again. This time it should work, and should
	// return to the root page.
	switch_submenu(&menu, &b);
	TEST_ASSERT_EQUAL_STRING("This is a menu!", menu.curr->desc);

	// Now change the selected index to 1. Should move it into the
	// Option 2 page.
	menu.curr->selected = 1;
	switch_submenu(&menu, &a);
	TEST_ASSERT_EQUAL_STRING("Option 2", menu.curr->desc);

	// Now switch back again. This time make sure the root and curr pages are
	// equal.
	switch_submenu(&menu, &b);
	TEST_ASSERT_EQUAL_UINT64(menu.curr, menu.root);
}


void test_givenCursorIsMoved_should_correctlyWrapAroundOptions(void)
{
	add_submenu_option(menu.curr, "Option 1", switch_submenu);
	add_submenu_option(menu.curr, "Option 2", switch_submenu);
	add_submenu_option(menu.curr, "Option 3", switch_submenu);
	add_submenu_option(menu.curr, "Option 4", switch_submenu);

	TEST_ASSERT_EQUAL_UINT32(0, menu.curr->selected);

	move_menu_cursor(&menu, BIT(7));
	TEST_ASSERT_EQUAL_UINT32(1, menu.curr->selected);

	move_menu_cursor(&menu, BIT(7));
	TEST_ASSERT_EQUAL_UINT32(2, menu.curr->selected);

	move_menu_cursor(&menu, BIT(7));
	TEST_ASSERT_EQUAL_UINT32(3, menu.curr->selected);

	// First significant check. Should wrap around and be 0.
	move_menu_cursor(&menu, BIT(7));
	TEST_ASSERT_EQUAL_UINT32(0, menu.curr->selected); 

	// Now move back up. Should wrap around and be 3.
	move_menu_cursor(&menu, BIT(6));
	TEST_ASSERT_EQUAL_UINT32(3, menu.curr->selected);

	// Now move back up to 0.
	move_menu_cursor(&menu, BIT(6));
	TEST_ASSERT_EQUAL_UINT32(2, menu.curr->selected);

	move_menu_cursor(&menu, BIT(6));
	TEST_ASSERT_EQUAL_UINT32(1, menu.curr->selected);

	move_menu_cursor(&menu, BIT(6));
	TEST_ASSERT_EQUAL_UINT32(0, menu.curr->selected);
}

void noop_action(void *menu, void *data) { return; }

void test_givenNestedPage_whenReset_shouldOnlyBePageAndAction(void)
{
	Submenu *root = menu.curr;
	Submenu *target = NULL, *sub_target = NULL;

	target = add_submenu_option(root, "Reset target", switch_submenu);

	sub_target = add_submenu_option(target, "Child A", switch_submenu);
	add_submenu_option(target, "Child B", switch_submenu);
	add_submenu_option(sub_target, "Grandchild A1", switch_submenu);

	TEST_ASSERT_EQUAL_STRING("Reset target", target->desc);
	TEST_ASSERT_EQUAL_PTR(root, target->prev);
	TEST_ASSERT_EQUAL_UINT32(2, target->len);
	TEST_ASSERT_NOT_NULL(target->options);
	TEST_ASSERT_EQUAL_STRING("Child A", target->options[0]->desc);
	TEST_ASSERT_EQUAL_STRING("Child B", target->options[1]->desc);

	// Destroy all children
	kill_children_reset_page(target, noop_action);

	// Make sure everything has been reset.
	// NOTE: This does not reset `data`, as the responsibility for the entire
	// implementation of what data represents has been placed on the user.
	// In the future, there may be a function you can hook into to set and
	// unset data, but for the time being leave as is.
	TEST_ASSERT_EQUAL_STRING("Reset target", target->desc);
	TEST_ASSERT_EQUAL_UINT32(0, target->len);
	TEST_ASSERT_EQUAL_UINT32(1, target->cap);
	TEST_ASSERT_EQUAL_UINT32(0, target->selected);
	TEST_ASSERT_EQUAL_UINT32(0, target->dealloc_idx);
	TEST_ASSERT_EQUAL_UINT32(0, target->view_top);
	TEST_ASSERT_EQUAL_PTR(root, target->prev);
	TEST_ASSERT_NULL(target->options);
	TEST_ASSERT_EQUAL_PTR(target->action, noop_action);

	// Target remains usable after reset
	add_submenu_option(target, "After reset child", switch_submenu);
	TEST_ASSERT_EQUAL_UINT32(1, target->len);
	TEST_ASSERT_NOT_NULL(target->options);
	TEST_ASSERT_EQUAL_STRING("After reset child", target->options[0]->desc);
}


void test_ifCursorIsMovedAndSubmenuIsChanged_should_enterCorrectSubmenu(void)
{
	uint32_t a = BIT(0), b = BIT(1);

	add_submenu_option(menu.curr, "Option 1", switch_submenu);
	add_submenu_option(menu.curr, "Option 2", switch_submenu);

	// We know we can enter the first option so it doesn't need testing.
	// Instead, let's test the following instructions:
	// 1. Move cursor to Option 2.
	// 2. Enter Option 2 and check that it is Option 2.
	// 3. Exit to root.
	// 4. Move cursor DOWN which will wrap to Option 1.
	// 5. Enter Option 1 and check that it is Option 1.
	// 6. Exit back to root and check that it is root.

	// 1.
	move_menu_cursor(&menu, BIT(7));

	// 2.
	switch_submenu(&menu, &a);
	TEST_ASSERT_EQUAL_STRING("Option 2", menu.curr->desc); 

	// 3.
	switch_submenu(&menu, &b);

	// 4.
	move_menu_cursor(&menu, BIT(7));

	// 5.
	switch_submenu(&menu, &a);
	TEST_ASSERT_EQUAL_STRING("Option 1", menu.curr->desc);

	// 6.
	switch_submenu(&menu, &b);
	TEST_ASSERT_EQUAL_UINT64(menu.root, menu.curr);
}


int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_givenMenuIsInitialized_should_displayCorrectTitle);
	RUN_TEST(test_givenNewDescriptionAdded_should_correctlyChangeDescription);
	RUN_TEST(test_givenGetDescription_should_correctlyReturnDescription);
	RUN_TEST(test_givenAThirdLevelPage_should_beConstructedCorrectly);
	RUN_TEST(test_givenThreeSubmenusWithTwoLevelsEach_should_correctContainAllInformation);
	RUN_TEST(test_givenSubmenuIsChanged_then_newCurrentShouldBeUpdated);
	RUN_TEST(test_givenNestedPage_whenReset_shouldOnlyBePageAndAction);
	RUN_TEST(test_ifCursorIsMovedAndSubmenuIsChanged_should_enterCorrectSubmenu);
	UNITY_END();
}

