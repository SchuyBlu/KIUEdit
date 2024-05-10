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


void test_ifCidAndWipValid_should_returnCorrectString(void)
{
	// Test null values
	TEST_ASSERT_NULL(map_to_weapon(0, 12));
	TEST_ASSERT_NULL(map_to_weapon(9, 0));

	// Test all blades
	TEST_ASSERT_EQUAL_STRING("First Blade", map_to_weapon(0, 0));
	TEST_ASSERT_EQUAL_STRING("Burst Blade", map_to_weapon(0, 1));
	TEST_ASSERT_EQUAL_STRING("Viper Blade", map_to_weapon(0, 2));
	TEST_ASSERT_EQUAL_STRING("Crusader Blade", map_to_weapon(0, 3));
	TEST_ASSERT_EQUAL_STRING("Royal Blade", map_to_weapon(0, 4));
	TEST_ASSERT_EQUAL_STRING("Optical Blade", map_to_weapon(0, 5));
	TEST_ASSERT_EQUAL_STRING("Samurai Blade", map_to_weapon(0, 6));
	TEST_ASSERT_EQUAL_STRING("Bullet Blade", map_to_weapon(0, 7));
	TEST_ASSERT_EQUAL_STRING("Aquarius Blade", map_to_weapon(0, 8));
	TEST_ASSERT_EQUAL_STRING("Aurum Blade", map_to_weapon(0, 9));
	TEST_ASSERT_EQUAL_STRING("Palutena Blade", map_to_weapon(0, 10));
	TEST_ASSERT_EQUAL_STRING("Gaol Blade", map_to_weapon(0, 11));

	// test all staves
	TEST_ASSERT_EQUAL_STRING("Insight Staff", map_to_weapon(1, 0));
	TEST_ASSERT_EQUAL_STRING("Orb Staff", map_to_weapon(1, 1));
	TEST_ASSERT_EQUAL_STRING("Rose Staff", map_to_weapon(1, 2));
	TEST_ASSERT_EQUAL_STRING("Knuckle Staff", map_to_weapon(1, 3));
	TEST_ASSERT_EQUAL_STRING("Ancient Staff", map_to_weapon(1, 4));
	TEST_ASSERT_EQUAL_STRING("Lancer Staff", map_to_weapon(1, 5));
	TEST_ASSERT_EQUAL_STRING("Flintlock Staff", map_to_weapon(1, 6));
	TEST_ASSERT_EQUAL_STRING("Somewhat Staff", map_to_weapon(1, 7));
	TEST_ASSERT_EQUAL_STRING("Scorpio Staff", map_to_weapon(1, 8));
	TEST_ASSERT_EQUAL_STRING("Laser Staff", map_to_weapon(1, 9));
	TEST_ASSERT_EQUAL_STRING("Dark Pit Staff", map_to_weapon(1, 10));
	TEST_ASSERT_EQUAL_STRING("Thanatos Staff", map_to_weapon(1, 11));

	// test all claws
	TEST_ASSERT_EQUAL_STRING("Tiger Claws", map_to_weapon(2, 0));
	TEST_ASSERT_EQUAL_STRING("Wolf Claws", map_to_weapon(2, 1));
	TEST_ASSERT_EQUAL_STRING("Bear Claws", map_to_weapon(2, 2));
	TEST_ASSERT_EQUAL_STRING("Brawler Claws", map_to_weapon(2, 3));
	TEST_ASSERT_EQUAL_STRING("Stealth Claws", map_to_weapon(2, 4));
	TEST_ASSERT_EQUAL_STRING("Hedgehog Claws", map_to_weapon(2, 5));
	TEST_ASSERT_EQUAL_STRING("Raptor Claws", map_to_weapon(2, 6));
	TEST_ASSERT_EQUAL_STRING("Artillery Claws", map_to_weapon(2, 7));
	TEST_ASSERT_EQUAL_STRING("Cancer Claws", map_to_weapon(2, 8));
	TEST_ASSERT_EQUAL_STRING("Beam Claws", map_to_weapon(2, 9));
	TEST_ASSERT_EQUAL_STRING("Viridi Claws", map_to_weapon(2, 10));
	TEST_ASSERT_EQUAL_STRING("Pandora Claws", map_to_weapon(2, 11));

	// test all bows
	TEST_ASSERT_EQUAL_STRING("Fortune Bow", map_to_weapon(3, 0));
	TEST_ASSERT_EQUAL_STRING("Silver Bow", map_to_weapon(3, 1));
	TEST_ASSERT_EQUAL_STRING("Meteor Bow", map_to_weapon(3, 2));
	TEST_ASSERT_EQUAL_STRING("Divine Bow", map_to_weapon(3, 3));
	TEST_ASSERT_EQUAL_STRING("Darkness Bow", map_to_weapon(3, 4));
	TEST_ASSERT_EQUAL_STRING("Crystal Bow", map_to_weapon(3, 5));
	TEST_ASSERT_EQUAL_STRING("Angel Bow", map_to_weapon(3, 6));
	TEST_ASSERT_EQUAL_STRING("Hawkeye Bow", map_to_weapon(3, 7));
	TEST_ASSERT_EQUAL_STRING("Sagittarius Bow", map_to_weapon(3, 8));
	TEST_ASSERT_EQUAL_STRING("Aurum Bow", map_to_weapon(3, 9));
	TEST_ASSERT_EQUAL_STRING("Palutena Bow", map_to_weapon(3, 10));
	TEST_ASSERT_EQUAL_STRING("Phosphora Bow", map_to_weapon(3, 11));

	// test all palms
	TEST_ASSERT_EQUAL_STRING("Violet Palm", map_to_weapon(4, 0));
	TEST_ASSERT_EQUAL_STRING("Burning Palm", map_to_weapon(4, 1));
	TEST_ASSERT_EQUAL_STRING("Needle Palm", map_to_weapon(4, 2));
	TEST_ASSERT_EQUAL_STRING("Midnight Palm", map_to_weapon(4, 3));
	TEST_ASSERT_EQUAL_STRING("Cursed Palm", map_to_weapon(4, 4));
	TEST_ASSERT_EQUAL_STRING("Cutter Palm", map_to_weapon(4, 5));
	TEST_ASSERT_EQUAL_STRING("Pudgy Palm", map_to_weapon(4, 6));
	TEST_ASSERT_EQUAL_STRING("Ninja Palm", map_to_weapon(4, 7));
	TEST_ASSERT_EQUAL_STRING("Virgo Palm", map_to_weapon(4, 8));
	TEST_ASSERT_EQUAL_STRING("Aurum Palm", map_to_weapon(4, 9));
	TEST_ASSERT_EQUAL_STRING("Viridi Palm", map_to_weapon(4, 10));
	TEST_ASSERT_EQUAL_STRING("Great Reaper Palm", map_to_weapon(4, 11));

	// test all clubs
	TEST_ASSERT_EQUAL_STRING("Ore Club", map_to_weapon(5, 0));
	TEST_ASSERT_EQUAL_STRING("Babel Club", map_to_weapon(5, 1));
	TEST_ASSERT_EQUAL_STRING("Skyscraper Club", map_to_weapon(5, 2));
	TEST_ASSERT_EQUAL_STRING("Atlas Club", map_to_weapon(5, 3));
	TEST_ASSERT_EQUAL_STRING("Earthmaul Club", map_to_weapon(5, 4));
	TEST_ASSERT_EQUAL_STRING("Ogre Club", map_to_weapon(5, 5));
	TEST_ASSERT_EQUAL_STRING("Halo Club", map_to_weapon(5, 6));
	TEST_ASSERT_EQUAL_STRING("Black Club", map_to_weapon(5, 7));
	TEST_ASSERT_EQUAL_STRING("Capricorn Club", map_to_weapon(5, 8));
	TEST_ASSERT_EQUAL_STRING("Aurum Club", map_to_weapon(5, 9));
	TEST_ASSERT_EQUAL_STRING("Hewdraw Club", map_to_weapon(5, 10));
	TEST_ASSERT_EQUAL_STRING("Magnus Club", map_to_weapon(5, 11));

	// test all cannons
	TEST_ASSERT_EQUAL_STRING("EZ Cannon", map_to_weapon(6, 0));
	TEST_ASSERT_EQUAL_STRING("Ball Cannon", map_to_weapon(6, 1));
	TEST_ASSERT_EQUAL_STRING("Predator Cannon", map_to_weapon(6, 2));
	TEST_ASSERT_EQUAL_STRING("Poseidon Cannon", map_to_weapon(6, 3));
	TEST_ASSERT_EQUAL_STRING("Fireworks Cannon", map_to_weapon(6, 4));
	TEST_ASSERT_EQUAL_STRING("Rail Cannon", map_to_weapon(6, 5));
	TEST_ASSERT_EQUAL_STRING("Dynamo Cannon", map_to_weapon(6, 6));
	TEST_ASSERT_EQUAL_STRING("Doom Cannon", map_to_weapon(6, 7));
	TEST_ASSERT_EQUAL_STRING("Leo Cannon", map_to_weapon(6, 8));
	TEST_ASSERT_EQUAL_STRING("Sonic Cannon", map_to_weapon(6, 9));
	TEST_ASSERT_EQUAL_STRING("Twinbellows Cannon", map_to_weapon(6, 10));
	TEST_ASSERT_EQUAL_STRING("Cragalanche Cannon", map_to_weapon(6, 11));

	// test all orbitars
	TEST_ASSERT_EQUAL_STRING("Standard Orbitars", map_to_weapon(7, 0));
	TEST_ASSERT_EQUAL_STRING("Guardian Orbitars", map_to_weapon(7, 1));
	TEST_ASSERT_EQUAL_STRING("Shock Orbitars", map_to_weapon(7, 2));
	TEST_ASSERT_EQUAL_STRING("Eyetrack Orbitars", map_to_weapon(7, 3));
	TEST_ASSERT_EQUAL_STRING("Fairy Orbitars", map_to_weapon(7, 4));
	TEST_ASSERT_EQUAL_STRING("Paw Pad Orbitars", map_to_weapon(7, 5));
	TEST_ASSERT_EQUAL_STRING("Jetstream Orbitars", map_to_weapon(7, 6));
	TEST_ASSERT_EQUAL_STRING("Boom Orbitars", map_to_weapon(7, 7));
	TEST_ASSERT_EQUAL_STRING("Gemini Orbitars", map_to_weapon(7, 8));
	TEST_ASSERT_EQUAL_STRING("Aurum Orbitars", map_to_weapon(7, 9));
	TEST_ASSERT_EQUAL_STRING("Centurion Orbitars", map_to_weapon(7, 10));
	TEST_ASSERT_EQUAL_STRING("Arlon Orbitars", map_to_weapon(7, 11));

	// test all arms
	TEST_ASSERT_EQUAL_STRING("Crusher Arm", map_to_weapon(8, 0));
	TEST_ASSERT_EQUAL_STRING("Compact Arm", map_to_weapon(8, 1));
	TEST_ASSERT_EQUAL_STRING("Electroshock Arm", map_to_weapon(8, 2));
	TEST_ASSERT_EQUAL_STRING("Volcano Arm", map_to_weapon(8, 3));
	TEST_ASSERT_EQUAL_STRING("Drill Arm", map_to_weapon(8, 4));
	TEST_ASSERT_EQUAL_STRING("Bomber Arm", map_to_weapon(8, 5));
	TEST_ASSERT_EQUAL_STRING("Bowl Arm", map_to_weapon(8, 6));
	TEST_ASSERT_EQUAL_STRING("End-All Arm", map_to_weapon(8, 7));
	TEST_ASSERT_EQUAL_STRING("Taurus Arm", map_to_weapon(8, 8));
	TEST_ASSERT_EQUAL_STRING("Upperdash Arm", map_to_weapon(8, 9));
	TEST_ASSERT_EQUAL_STRING("Kraken Arm", map_to_weapon(8, 10));
	TEST_ASSERT_EQUAL_STRING("Phoenix Arm", map_to_weapon(8, 11));
}


void test_givenFirstBladeAndSixStars_should_containCorrectInfo(void)
{
	SaveFile tsave;
	Weapon *first_blade = NULL;

	tsave.fp = fopen("test/input/file/first_blade_6r.bin", "r+");
	if (!tsave.fp) {
		fprintf(stderr, "Error opening first blade file.\n");
		return;
	}

	// Now run tests here. Name and stars should be populated after this call.
	first_blade = fetch_savefile_weapon(&tsave, 0x0);

	TEST_ASSERT_EQUAL_STRING("First Blade", first_blade->name);
	TEST_ASSERT_EQUAL_FLOAT(6.0, (float)first_blade->ranged / 2);
	TEST_ASSERT_EQUAL_FLOAT(0.0, (float)first_blade->melee / 2);

	fclose(tsave.fp);
	destroy_weapon(first_blade);
}


void test_givenGaolBladeAndBothSixStars_should_containCorrectInfo(void)
{
	SaveFile tsave;
	Weapon *gaol_blade = NULL;

	tsave.fp = fopen("test/input/file/gaol_blade_6r6m.bin", "r+");
	if (!tsave.fp) {
		fprintf(stderr, "Error opening gaol blade file.\n");
		return;
	}

	// Now run tests here. Name and stars should be populated after this call.
	gaol_blade = fetch_savefile_weapon(&tsave, 0x0);

	TEST_ASSERT_EQUAL_STRING("Gaol Blade", gaol_blade->name);
	TEST_ASSERT_EQUAL_FLOAT(6.0, (float)gaol_blade->ranged / 2);
	TEST_ASSERT_EQUAL_FLOAT(6.0, (float)gaol_blade->melee / 2);

	fclose(tsave.fp);
	destroy_weapon(gaol_blade);
}


void test_givenDrillArmAndOnlyFourHalvesMelee_should_containCorrectInfo(void)
{
	SaveFile tsave;
	Weapon *drill_arm = NULL;

	tsave.fp = fopen("test/input/file/drill_arm_6m.bin", "r+");
	if (!tsave.fp) {
		fprintf(stderr, "Error opening drill arm file.\n");
		return;
	}

	// Now run tests here. Name and stars should be populated after this call.
	drill_arm = fetch_savefile_weapon(&tsave, 0x0);

	TEST_ASSERT_EQUAL_STRING("Drill Arm", drill_arm->name);
	TEST_ASSERT_EQUAL_FLOAT(0.0, (float)drill_arm->ranged / 2);
	TEST_ASSERT_EQUAL_FLOAT(4.5, (float)drill_arm->melee / 2);

	fclose(tsave.fp);
	destroy_weapon(drill_arm);
}


void test_givenMetaDataAndStars_should_combineToCorrectBytes(void)
{
	uint8_t cid = 0, wid = 0;
	uint16_t correct_bytes = 0x219c, result_bytes = 0;

	Weapon *weapon = fetch_savefile_weapon(&save, 0x670);

	cid = (weapon->ids & 0xf0) >> 4;
	wid = weapon->ids & 0x0f;

	result_bytes |= (uint16_t)cid << 2;
	result_bytes |= (uint16_t)wid << 7;
	result_bytes |= weapon->metadata;
	destroy_weapon(weapon);

	TEST_ASSERT_EQUAL_UINT16(correct_bytes, result_bytes);
}


void test_givenCookieCutterDrillArm_should_mapToCorrectMods(void)
{
	SaveFile tsave;
	Weapon *drill_arm = NULL;

	tsave.fp = fopen("test/input/file/drill_arm_cookie.bin", "r+");
	if (!tsave.fp) {
		fprintf(stderr, "Error opening drill arm file.\n");
		return;
	}

	drill_arm = fetch_savefile_weapon(&tsave, 0x0);

	TEST_ASSERT_EQUAL_STRING("Drill Arm", drill_arm->name);
	TEST_ASSERT_EQUAL_FLOAT(4.5, (float)drill_arm->ranged / 2);
	TEST_ASSERT_EQUAL_UINT8(0, drill_arm->melee);
	TEST_ASSERT_EQUAL_STRING("Shot Range +3", drill_arm->mod1);
	TEST_ASSERT_EQUAL_STRING("Shot Homing +3", drill_arm->mod2);
	TEST_ASSERT_EQUAL_STRING("Evasion +4", drill_arm->mod3);
	TEST_ASSERT_EQUAL_STRING("Dash ch. shot +4", drill_arm->mod4);
	TEST_ASSERT_EQUAL_STRING("Effect Duration +4", drill_arm->mod5);
	TEST_ASSERT_EQUAL_STRING("Overall Defense -4", drill_arm->mod6);

	fclose(tsave.fp);
	destroy_weapon(drill_arm);
}


int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ifSaveIsInitializedProperlyThenFile_should_beTruthy);
	RUN_TEST(test_ifHeartsAreRead_should_returnProperHeartCount);
	RUN_TEST(test_ifSaveIsInitializedProperly_should_haveAllProperHeartData);
	RUN_TEST(test_assertModStringLen_should_MatcheModValueLen);
	RUN_TEST(test_ifCidPassed_should_returnCorrectArray);
	RUN_TEST(test_ifCidAndWipValid_should_returnCorrectString);
	RUN_TEST(test_givenFirstBladeAndSixStars_should_containCorrectInfo);
	RUN_TEST(test_givenGaolBladeAndBothSixStars_should_containCorrectInfo);
	RUN_TEST(test_givenDrillArmAndOnlyFourHalvesMelee_should_containCorrectInfo);
	RUN_TEST(test_givenMetaDataAndStars_should_combineToCorrectBytes);
	RUN_TEST(test_givenCookieCutterDrillArm_should_mapToCorrectMods);
	UNITY_END();
}

