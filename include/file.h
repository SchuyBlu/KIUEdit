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
 * Retrieves weapons from the file and stores them into a 64 bit boundary.
 * `save` - save file struct to be saved into.
 */
void fetch_savefile_weapon(SaveFile *save);

