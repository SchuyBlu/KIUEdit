/*
 * Author: Schuy
 * A collection of data types and constants relating to weapons and weapon
 * definitions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


/*
 * Struct to store both binary data for a Weapon and strings to make it
 * human readable.
 * `metadata` - two bytes pertaining to time obtained.
 * `ranged` - contains how many ranged stars * 2.
 * `melee` - contains how many melee stars * 2.
 * `bin_mod1` - binary representation of first mod slot.
 * `bin_mod2` - binary representation of second mod slot.
 * `bin_mod3` - binary representation of third mod slot.
 * `bin_mod4` - binary representation of fourth mod slot.
 * `bin_mod5` - binary representation of fifth mod slot.
 * `bin_mod6` - binary representation of sixth mod slot.
 * `name` - string representation of weapon name.
 * `mod1` - string representation of first mod slot.
 * `mod2` - string representation of second mod slot.
 * `mod3` - string representation of third mod slot.
 * `mod4` - string representation of fourth mod slot.
 * `mod5` - string representation of fifth mod slot.
 * `mod6` - string representation of sixth mod slot.
 */
typedef struct Weapon {
	// Regions to hold necessary binary data.
	uint8_t metadata;
	uint8_t ids;
	uint8_t bin_mod1;
	uint8_t bin_mod2;
	uint8_t bin_mod3;
	uint8_t bin_mod4;
	uint8_t bin_mod5;
	uint8_t bin_mod6;

	// Region to hold human readaable data.
	const char *name; // Stack allocated, do not free
	uint8_t ranged;
	uint8_t melee;
	char *mod1;
	char *mod2;
	char *mod3;
	char *mod4;
	char *mod5;
	char *mod6;
} Weapon;


/*
 * Initializes the values in a weapon struct.
 * `weapon` - Weapon struct being initialized.
 * Returns: pointer to Weapon struct.
 */
Weapon *weapon_init(Weapon *weapon);


/*
 * Destroys the data in a weapon struct.
 * `weapon` - Weapon struct being destroyed.
 */
void destroy_weapon(Weapon *weapon);
