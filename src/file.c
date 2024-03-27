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


void fetch_savefile_weapons(SaveFile *save)
{
	uint64_t region = 0;
	Weapon *weapon = NULL;

	weapon = weapon_init(weapon);

	fseek(save->fp, WEAPON_OFFSET + 0x5, SEEK_SET);
	
	// Read 8 bytes from this point, which can hold weapon type and stars.
	fread(&region, 8, 1, save->fp);

	printf("%lx\n", region);

	destroy_weapon(weapon);
}

