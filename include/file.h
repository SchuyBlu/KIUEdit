/*
 * Author: Schuy
 * Meant to handle all functions related to loading save data from file.
 */

// Set up defines for where json data is held depending on if the build is
// a testing build or a release build.
#ifdef TEST_BUILD
#define GAME_DATA_PATH "data"
#else
#define GAME_DATA_PATH "data"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "offsets.h"
#include "weapons.h"
#include "const.h"


/*
 * Struct meant to contain all relevant save file data for Kid Icarus
 * Uprising.
 * `fp` - file pointer to save file.
 * `hearts` - hearts in save file.
 * `donate_p` - palutena heart donations.
 * `donate_v` - viridi heart donations.
 */
typedef struct SaveFile {
	FILE *fp;
	uint32_t hearts;
	uint32_t donate_p;
	uint32_t donate_v;
} SaveFile;


/*
 * Initializes save file pointers to `NULL` and integers to zero.
 * `save` - pointer to save file struct to be initialized.
 * `path` - string representing path to save file.
 */
void savefile_init(SaveFile *save, char *path);


/*
 * Destroys all remaining save file struct data.
 * `save` - pointer to save file struct to be destroyed.
 */
void destroy_savefile(SaveFile *save);


/*
 * Retrieves and returns hearts from varying locations and returns as a u32 int.
 * `save` - save file struct.
 * `offset` - offset to start reading from in save file.
 */
uint32_t _retrieve_hearts(SaveFile *save, D_OFFSET offset);


/*
 * Retrieves save file hearts and stores them in `SaveFile struct` passed to
 * function.
 * `save` - save file struct to be saved into.
 */
 void fetch_savefile_hearts(SaveFile *save);


/*
 * Returns an array of strings of weapons within a certain class by mapping
 * class id to said array.
 * `cid` - class id.
 * Returns:
 * `const char **const` - Array of weapon names within class.
 */
const char **const map_to_class(uint8_t cid);


/*
 * Returns a constant string representation of whatever weapon is determined
 * by cid and wid.
 * `cid` - class id.
 * `wid` - weapon id.
 * Returns:
 * `const char *const` - immutable string containing name of weapon.
 */
const char *const map_to_weapon(uint8_t cid, uint8_t wid);


/*
 * Internal function for populating name data for weapon.
 * `weapon` - weapon whose data is being populated.
 * `data` - data pertaining to the name.
 */
static void populate_name_data(Weapon *weapon, uint32_t data);


/*
 * Interal function for populating star data for weapon.
 * `weapon` - weapon whose data is being populated.
 * `data` - data pertaining to stars.
 */
static void populate_star_data(Weapon *weapon, uint32_t data);


/*
 * Maps a value to a string by accessed MOD_STRINGS (const.c). The value
 * is the index required + 1.
 * `slot` - double pointer to string, to allow modifying in place.
 * `val` - Index + 1 of the string.
 */
static void populate_mod_check(const char **slot, uint8_t val);


/*
 * Internal function for populating mod data for weapon.
 * `weapon` - weapon whose data is being populated.
 * `fp` - Pointer to file being read from.
 */
static void populate_mod_data(Weapon *weapon, FILE *fp);

/*
 * Retrieves weapon from the file and returns it..
 * `save` - save file struct to be saved into.
 * `offset` - offset of weapon being read.
 * Returns:
 * `Weapon*` - pointer to weapon struct.
 */
Weapon *fetch_savefile_weapon(SaveFile *save, uint32_t offset);

