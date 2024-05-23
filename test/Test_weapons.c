#include "weapons.h"
#include "unity.h"

Weapon weapon;

void setUp()
{
	weapon_init(&weapon);
	weapon.ranged = 9;
	weapon.melee = 0;
	weapon.bin_mod1 = 0xb5; // Shot Range +3
	weapon.bin_mod2 = 0xbb; // Shot Homing +3
	weapon.bin_mod3 = 0x66; // Evasion +4
	weapon.bin_mod4 = 0x0a; // Dash ch. shot +4
	weapon.bin_mod5 = 0x92; // Effect Duration +4
	weapon.bin_mod6 = 0x8a; // Overall Defense -4
}


void tearDown()
{
	return;
}


void test_givenWeapon_should_reportCorrectValue()
{
	TEST_ASSERT_EQUAL_UINT32(310, weapon_get_value(&weapon));
}


int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_givenWeapon_should_reportCorrectValue);
	UNITY_END();
}

