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
	int len = 0;
	char **list = get_save_strings("test/test_dirs/test_all", &len);

	TEST_ASSERT_EQUAL_INT(4, len);

	for(int i = 0; i < len; i++)
		free(list[i]);
	free(list);
}


void test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectFilenames(void)
{
	int len = 0;
	char **list = get_save_strings("test/test_dirs/test_all", &len);
	
	TEST_ASSERT_EQUAL_STRING("00.SAV", list[0]);
	TEST_ASSERT_EQUAL_STRING("01.SAV", list[1]);
	TEST_ASSERT_EQUAL_STRING("02.SAV", list[2]);
	TEST_ASSERT_EQUAL_STRING("03.SAV", list[3]);

	for (int i = 0; i < len; i++)
		free(list[i]);
	free(list);
}


void test_whenDirectoryIsLoadedWithOneFile_should_containCorrectLenAndName(void)
{
	int len = 0;
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

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectLength);
	RUN_TEST(test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectFilenames);
	RUN_TEST(test_whenWeaponsAreLoadedIntoSave_should_containCorrectWeaponNames);
	UNITY_END();
}

