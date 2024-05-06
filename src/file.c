#include "file.h"


void savefile_init(SaveFile *save, char *path)
{
	save->fp = NULL;

	// Attempt to open save file.
	save->fp = fopen(path, "r+");
	assert(save->fp);

	fetch_savefile_hearts(save);
}


void destroy_savefile(SaveFile *save)
{
	fclose(save->fp);
}


uint32_t _retrieve_hearts(SaveFile *save, D_OFFSET offset)
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
	save->hearts = _retrieve_hearts(save, HEART_OFFSET);
	save->donate_p = _retrieve_hearts(save, PALU_OFFSET);
	save->donate_v = _retrieve_hearts(save, VIRIDI_OFFSET);
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


void fetch_savefile_weapon(SaveFile *save, uint32_t offset)
{
	uint8_t wid = 0, cid = 0;
	uint32_t region = 0;
	Weapon *weapon = NULL;

	weapon = weapon_init(weapon);

	fseek(save->fp, offset + 0x5, SEEK_SET);
	
	// Read 2 bytes from this point, which can hold weapon type and stars.
	fread(&region, 2, 1, save->fp);

	// Fetch data related to class and weapon ids.
	wid = (region >> 7) & 0x0f;
	cid = (region >> 2) & 0x0f;

	// Populate data into weapon.
	weapon->metadata = region & 0xf843; // mask for data not including cid and wid.
	weapon->ids = (cid << 4) | wid;
	weapon->name = map_to_weapon(cid, wid);

	destroy_weapon(weapon);
}

