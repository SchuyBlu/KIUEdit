#include "file.h"

void savefile_init(SaveFile *save, char *path)
{
	save->fp = NULL;
	save->weapons = (Weapons){ NULL, 0, 1 };

	save->fp = fopen(path, "r+");
	assert(save->fp);

	fetch_savefile_hearts(save);
	populate_savefile_weapons(save, 0x670);
}


void destroy_savefile(SaveFile *save)
{
	if (!save->weapons.array)
		return;

	if (save->weapons.len == 0)
		return;

	for (int i = 0; i < save->weapons.len; i++)
		free(save->weapons.array[i]);

	free(save->weapons.array);
}


uint32_t retrieve_hearts(SaveFile *save, D_OFFSET offset)
{
	// Set heart buffer to three because I'm only seeking out 3 bytes
	uint32_t hearts = 0;

	// Read three bytes after heart offset into hearts variable
	fseek(save->fp, offset, SEEK_SET);
	fread(&hearts, 3, 1, save->fp);

	return hearts;
}


void fetch_savefile_hearts(SaveFile *save)
{
	save->hearts = retrieve_hearts(save, HEART_OFFSET);
	save->donate_p = retrieve_hearts(save, PALU_OFFSET);
	save->donate_v = retrieve_hearts(save, VIRIDI_OFFSET);
}


const char **const map_to_class(uint8_t cid)
{
	switch (cid) {
		case 0:
			return BLADE_STRINGS;
		case 1:
			return STAFF_STRINGS;
		case 2:
			return CLAW_STRINGS;
		case 3:
			return BOW_STRINGS;
		case 4:
			return PALM_STRINGS;
		case 5:
			return CLUB_STRINGS;
		case 6:
			return CANNON_STRINGS;
		case 7:
			return ORBITAR_STRINGS;
		case 8:
			return ARM_STRINGS;
		default:
			return NULL;
	}
}


const char *const map_to_weapon(uint8_t cid, uint8_t wid)
{
	const char **const weapons = map_to_class(cid);

	if (weapons == NULL || wid > 11)
		return NULL;

	return weapons[wid];
}


bool check_if_weapon_present(SaveFile *save, uint32_t offset)
{
	uint32_t result = 0;

	fseek(save->fp, offset + 5, SEEK_SET);
	fread(&result, 2, 1, save->fp);

	return result != 0;
}


void populate_name_data(Weapon *weapon, uint32_t data) 
{
	uint8_t wid = 0, cid = 0;

	// Fetch data related to class and weapon ids.
	wid = (data >> 7) & 0x0f;
	cid = (data >> 2) & 0x0f;

	// Populate data related to name, ids, and timestamp metadata into weapon.
	weapon->metadata = data & WEP_MASK; // mask for data not including cid and wid.
	weapon->ids = (cid << 4) | wid;
	weapon->name = map_to_weapon(cid, wid);
}


void populate_star_data(Weapon *weapon, uint32_t data)
{
	uint8_t star_data1 = 0, star_data2 = 0;

	star_data1 = data >> 8;
	star_data2 = data >> 24;

	// determines if first star is melee star.
	// If so, store as melee.
	if (star_data1 & 0x10) {
		weapon->melee = star_data1 & 0x0f;
		weapon->ranged = 0;
	} else {
		weapon->ranged = star_data1 & 0x0f;
		weapon->melee = star_data2 & 0x0f;
	}
}


void populate_mod_string(const char **slot, uint8_t index)
{
	if (index == 0)
		return;

	*slot = (const char *const)MOD_STRINGS[index - 1];
}


void populate_mod_data(Weapon *weapon, FILE *fp)
{
	fread(&weapon->bin_mod1, 2, 1, fp);
	fread(&weapon->bin_mod2, 2, 1, fp);
	fread(&weapon->bin_mod3, 2, 1, fp);
	fread(&weapon->bin_mod4, 2, 1, fp);
	fread(&weapon->bin_mod5, 2, 1, fp);
	fread(&weapon->bin_mod6, 2, 1, fp);

	populate_mod_string(&weapon->mod1, weapon->bin_mod1);
	populate_mod_string(&weapon->mod2, weapon->bin_mod2);
	populate_mod_string(&weapon->mod3, weapon->bin_mod3);
	populate_mod_string(&weapon->mod4, weapon->bin_mod4);
	populate_mod_string(&weapon->mod5, weapon->bin_mod5);
	populate_mod_string(&weapon->mod6, weapon->bin_mod6);
}


Weapon *fetch_savefile_weapon(SaveFile *save, uint32_t offset)
{
	uint32_t region = 0;
	Weapon *weapon = NULL;

	weapon = weapon_init(weapon);

	fseek(save->fp, offset + 0x5, SEEK_SET);
	
	// Read 2 bytes from this point, which can hold weapon type.
	fread(&region, 2, 1, save->fp);
	populate_name_data(weapon, region);

	// We are now interested in star data, read star data.
	region = 0;
	fread(&region, 4, 1, save->fp);
	populate_star_data(weapon, region);

	// We are now interested in mod data.
	fseek(save->fp, offset + 0x14, SEEK_SET);
	populate_mod_data(weapon, save->fp);

	return weapon;
}

void populate_savefile_weapons(SaveFile *save, uint32_t offset)
{
	// If is null, allocate memory.
	if (!save->weapons.array) {
		save->weapons.array = malloc(sizeof(Weapon*) * save->weapons.cap);
		assert(save->weapons.array);
		save->weapons.array[0] = NULL;
	}

	// Now read in first weapon.
	int count = 0;
	while (check_if_weapon_present(save, offset) && count != 999) {
		// Reallocate if len matches capacity.
		if (save->weapons.len == save->weapons.cap) {
			save->weapons.cap *= 2;
			save->weapons.array = realloc(save->weapons.array, sizeof(Weapon*) * save->weapons.cap);
			assert(save->weapons.array);

			// NULL initialize array.
			for (int i = save->weapons.len; i < save->weapons.cap; i++)
				save->weapons.array[i] = NULL;
		}

		save->weapons.array[save->weapons.len] = fetch_savefile_weapon(save, offset);
		save->weapons.len++;
		offset += NEXT_WEAPON;
		count++;
	}
}
