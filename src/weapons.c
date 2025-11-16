#include "weapons.h"


Weapon *weapon_init(Weapon *weapon)
{
	if (weapon == NULL) {
		weapon = malloc(sizeof(Weapon));
		assert(weapon);
	}

	weapon->name = NULL;
	weapon->metadata = 0;
	weapon->ids = 0;
	weapon->ranged = 0;
	weapon->melee = 0;
	weapon->bin_mod1 = 0;
	weapon->bin_mod2 = 0;
	weapon->bin_mod3 = 0;
	weapon->bin_mod4 = 0;
	weapon->bin_mod5 = 0;
	weapon->bin_mod6 = 0;

	weapon->mod1 = NULL;
	weapon->mod2 = NULL;
	weapon->mod3 = NULL;
	weapon->mod4 = NULL;
	weapon->mod5 = NULL;
	weapon->mod6 = NULL;

	return weapon;
}


uint32_t weapon_get_value(Weapon *weapon)
{
	float value = 100;

	value += weapon->ranged ? RANGED_VALUES[weapon->ranged] : 0;
	value += weapon->melee ? MELEE_VALUES[weapon->melee] : 0;
	value += weapon->bin_mod1 ? MOD_VALUES[weapon->bin_mod1 - 1] : 0;
	value += weapon->bin_mod2 ? MOD_VALUES[weapon->bin_mod2 - 1] : 0;
	value += weapon->bin_mod3 ? MOD_VALUES[weapon->bin_mod3 - 1] : 0;
	value += weapon->bin_mod4 ? MOD_VALUES[weapon->bin_mod4 - 1] : 0;
	value += weapon->bin_mod5 ? MOD_VALUES[weapon->bin_mod5 - 1] : 0;
	value += weapon->bin_mod6 ? MOD_VALUES[weapon->bin_mod6 - 1] : 0;

	return (uint32_t)value;
}


void destroy_weapon(Weapon *weapon)
{
	free(weapon);
}

