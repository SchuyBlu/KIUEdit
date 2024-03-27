#include "weapons.h"


Weapon *weapon_init(Weapon *weapon)
{
	if (weapon == NULL) {
		weapon = malloc(sizeof(Weapon));
		assert(weapon);
	}

	weapon->bin_type = 0;
	weapon->ranged = 0;
	weapon->melee = 0;
	weapon->bin_mod1 = 0;
	weapon->bin_mod2 = 0;
	weapon->bin_mod3 = 0;
	weapon->bin_mod4 = 0;
	weapon->bin_mod5 = 0;
	weapon->bin_mod6 = 0;

	weapon->name = NULL;
	weapon->mod1 = NULL;
	weapon->mod2 = NULL;
	weapon->mod3 = NULL;
	weapon->mod4 = NULL;
	weapon->mod5 = NULL;
	weapon->mod6 = NULL;

	return weapon;
}


void destroy_weapon(Weapon *weapon)
{
	// If one internal element has been allocated for, safely assume they all
	// have been.
	if (weapon->name != NULL) {
		free(weapon->name);
		free(weapon->mod1);
		free(weapon->mod2);
		free(weapon->mod3);
		free(weapon->mod4);
		free(weapon->mod5);
		free(weapon->mod6);
	}

	free(weapon);
}

