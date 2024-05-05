#define IS_TEST
#include "file.h"
#include "unity.h"
#include "const.h"


SaveFile save;


void setUp()
{
	savefile_init(&save, "test/input/in.sav");
}


void tearDown()
{
	destroy_savefile(&save);
}


void test_ifSaveIsInitializedProperlyThenFile_should_beTruthy(void)
{
	TEST_ASSERT_TRUE(save.fp);
}


void test_ifHeartsAreRead_should_returnProperHeartCount(void)
{
	uint32_t hearts;
	hearts = _retrieve_hearts(&save, HEART_OFFSET);
	TEST_ASSERT_EQUAL_UINT32(9853047, hearts);
}


void test_ifSaveIsInitializedProperly_should_haveAllProperHeartData(void)
{
	TEST_ASSERT_EQUAL_UINT32(9853047, save.hearts);
	TEST_ASSERT_EQUAL_UINT32(9999999, save.donate_p);
	TEST_ASSERT_EQUAL_UINT32(0, save.donate_v);
}


void test_assertModStringLen_should_MatcheModValueLen(void)
{
	// mod_strings_len and mod_values_len defined in const.c, length had to
	// be determined within file arrays were defined in.

	// #juststackissueslmao

	TEST_ASSERT_EQUAL_size_t(mod_strings_len, mod_values_len);
}


void test_ifCidPassed_should_returnCorrectArray(void)
{
	TEST_ASSERT_EQUAL_STRING("First Blade", map_to_class(0)[0]);
	TEST_ASSERT_EQUAL_STRING("Insight Staff", map_to_class(1)[0]);
	TEST_ASSERT_EQUAL_STRING("Tiger Claws", map_to_class(2)[0]);
	TEST_ASSERT_EQUAL_STRING("Fortune Bow", map_to_class(3)[0]);
	TEST_ASSERT_EQUAL_STRING("Violet Palm", map_to_class(4)[0]);
	TEST_ASSERT_EQUAL_STRING("Ore Club", map_to_class(5)[0]);
	TEST_ASSERT_EQUAL_STRING("EZ Cannon", map_to_class(6)[0]);
	TEST_ASSERT_EQUAL_STRING("Standard Orbitars", map_to_class(7)[0]);
	TEST_ASSERT_EQUAL_STRING("Crusher Arm", map_to_class(8)[0]);
	TEST_ASSERT_NULL(map_to_class(9));
}


void test_shouldReadWeaponNameCorrectly(void)
{
	fetch_savefile_weapon(&save);
}


int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ifSaveIsInitializedProperlyThenFile_should_beTruthy);
	RUN_TEST(test_ifHeartsAreRead_should_returnProperHeartCount);
	RUN_TEST(test_ifSaveIsInitializedProperly_should_haveAllProperHeartData);
	RUN_TEST(test_shouldReadWeaponNameCorrectly);
	RUN_TEST(test_assertModStringLen_should_MatcheModValueLen);
	RUN_TEST(test_ifCidPassed_should_returnCorrectArray);
	UNITY_END();
}

