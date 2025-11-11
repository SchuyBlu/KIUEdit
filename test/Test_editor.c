#include <string.h>
#include "editor.h"
#include "unity.h"

Menu menu;

void setUp()
{
	menu_init(&menu, "Testing Menu", switch_submenu);
	set_submenu_desc(menu.curr, "This is a menu!");
}


void tearDown()
{
	destroy_menu(&menu);
}


void test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectLength(void)
{
	uint32_t len = 0;
	char **list = get_save_strings("test/test_dirs/test_all", &len);

	for(int i = 0; i < len; i++)
		free(list[i]);
	free(list);

	TEST_ASSERT_EQUAL_INT(3, len);
}


void test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectFilenames(void)
{
	uint32_t len = 0;
	char **list = get_save_strings("test/test_dirs/test_all", &len);

	TEST_ASSERT_EQUAL_STRING("01.SAV", list[0]);
	TEST_ASSERT_EQUAL_STRING("02.SAV", list[1]);
	TEST_ASSERT_EQUAL_STRING("03.SAV", list[2]);

	for (int i = 0; i < len; i++)
		free(list[i]);
	free(list);
}


void test_whenDirectoryIsLoadedWithOneFile_should_containCorrectLenAndName(void)
{
	uint32_t len = 0;
	char **list = get_save_strings("test/test_dirs/test_one", &len);

	TEST_ASSERT_EQUAL_INT(1, len);
	TEST_ASSERT_EQUAL_STRING("01.SAV", list[0]);

	for (int i = 0; i < len; i++)
		free(list[i]);
	free(list);
}

void test_whenWeaponsAreLoadedIntoSave_should_containCorrectWeaponNames(void)
{
	SaveFile tsave;

	tsave.fp = NULL;
	tsave.weapons.array = NULL;
	tsave.weapons.cap = 1;
	tsave.weapons.len = 0;

	tsave.fp = fopen("test/input/file/weapon_list.bin", "r+");
	if (!tsave.fp) {
		fprintf(stderr, "Error opening max weapons file.\n");
		return;
	}

	populate_savefile_weapons(&tsave, 0x0);

	build_weapon_menus(menu.curr, tsave);

	TEST_ASSERT_EQUAL_UINT32(7, menu.curr->len);
	TEST_ASSERT_EQUAL_STRING("Eyetrack Orbitars", menu.curr->options[0]->desc);
	TEST_ASSERT_EQUAL_STRING("Fortune Bow", menu.curr->options[1]->desc);
	TEST_ASSERT_EQUAL_STRING("Predator Cannon", menu.curr->options[2]->desc);
	TEST_ASSERT_EQUAL_STRING("Bowl Arm", menu.curr->options[3]->desc);
	TEST_ASSERT_EQUAL_STRING("Stealth Claws", menu.curr->options[4]->desc);
	TEST_ASSERT_EQUAL_STRING("Jetstream Orbitars", menu.curr->options[5]->desc);
	TEST_ASSERT_EQUAL_STRING("Thanatos Staff", menu.curr->options[6]->desc);

	destroy_savefile(&tsave);
}

void test_editorInitCorrectly_should_correctlyLoadWeapons(void)
{
	Editor editor;

	editor_init(&editor);
	destroy_editor(&editor);
}

void press_a(Editor *e) { e->ctx.key_down = BIT(0); editor_switch_submenu(&e->menu, &e->ctx); }
void press_b(Editor *e) { e->ctx.key_down = BIT(1); editor_switch_submenu(&e->menu, &e->ctx); }

// Really unfortunate function. Needed to normalize path names for the test
// below alone. I know it's really hacky, but unfortunately it's necessary
// if I don't want to massively refactor (and I don't).
void norm_path_names(Editor *editor, char *base_dir)
{
	size_t need = 0;
	char *name = NULL, *full = NULL;

	for (int i = 0; i < editor->ctx.files_len; i++) {
		name = editor->ctx.files[i];
		need = strlen(base_dir) + strlen(name) + 1;
		full = malloc(sizeof(char) * need);
		assert(full);

		memcpy(full, base_dir, strlen(base_dir));
		memcpy(full + strlen(base_dir), name, strlen(name) + 1);

		free(editor->ctx.files[i]);
		editor->ctx.files[i] = full;
	}
}

void test_whenSavesAreLoaded_should_populateSubmenu_and_unloadOnFileChange(void)
{
	Editor editor;
	Submenu *opt0 = NULL, *opt1 = NULL;

	editor_init(&editor);
	norm_path_names(&editor, "test/input/editor/");

	TEST_ASSERT_TRUE_MESSAGE(editor.ctx.files_len >= 2, "Need at least two saves.");

	// At least one file option exists under root
	TEST_ASSERT_TRUE(editor.ctx.files_len >= 1);
	opt0 = editor.menu.root->options[0];
	opt1 = editor.menu.root->options[1];

	editor.menu.curr->selected = 0;
	press_a(&editor);

	// Now we should be inside that option.
	TEST_ASSERT_TRUE(editor.ctx.save_loaded);
	TEST_ASSERT_EQUAL_PTR(opt0, editor.menu.curr);
	// Ensures we can still navigate the menu and that the event action
	// has been changed.
	TEST_ASSERT_EQUAL_PTR(editor_switch_submenu, opt0->action);
	TEST_ASSERT_TRUE(opt0->len > 0);
	TEST_ASSERT_EQUAL_UINT32(0, editor.ctx.file_index);

	// Go back to root.
	press_b(&editor);
	TEST_ASSERT_EQUAL_PTR(editor.menu.root, editor.menu.curr);
	// Save is still loaded, actions unchanged
	TEST_ASSERT_TRUE(editor.ctx.save_loaded);
	TEST_ASSERT_EQUAL_PTR(editor_switch_submenu, opt0->action);
	TEST_ASSERT_EQUAL_PTR(load_save, opt1->action);

	// Select second save (opt1)
	editor.menu.curr->selected = 1;
	press_a(&editor);

	TEST_ASSERT_TRUE(editor.ctx.save_loaded);
	TEST_ASSERT_EQUAL_PTR(opt1, editor.menu.curr);
	TEST_ASSERT_EQUAL_PTR(editor_switch_submenu, opt1->action);
	TEST_ASSERT_TRUE(opt1->len > 0);
	TEST_ASSERT_EQUAL_UINT32(1, editor.ctx.file_index);

	// Old page cleared once a new save is loaded. Only one save
	// can be loaded at a time.
	TEST_ASSERT_EQUAL_UINT32(0, opt0->len);
	TEST_ASSERT_NULL(opt0->options);
	TEST_ASSERT_EQUAL_PTR(load_save, opt0->action);

	press_b(&editor);
	TEST_ASSERT_EQUAL_PTR(editor.menu.root, editor.menu.curr);
	TEST_ASSERT_TRUE(editor.ctx.save_loaded);
	// Note that they're swapped here compared to above.
	TEST_ASSERT_EQUAL_PTR(load_save, opt0->action);
	TEST_ASSERT_EQUAL_PTR(editor_switch_submenu, opt1->action);
	
	destroy_editor(&editor);
}


int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectLength);
	RUN_TEST(test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectFilenames);
	RUN_TEST(test_whenWeaponsAreLoadedIntoSave_should_containCorrectWeaponNames);
	RUN_TEST(test_editorInitCorrectly_should_correctlyLoadWeapons);
	RUN_TEST(test_whenSavesAreLoaded_should_populateSubmenu_and_unloadOnFileChange);
	UNITY_END();
}

