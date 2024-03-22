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
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <json-c/json.h>
#include "offsets.h"


/*
 * Struct meant to contain all relevant save file data for Kid Icarus
 * Uprising.
 * `fp` - file pointer to save file.
 * `hearts` - hearts in save file.
 * `donate_p` - palutena heart donations.
 * `donate_v` - viridi heart donations.
 */
struct SaveFile {
	FILE *fp;
	uint32_t hearts;
	uint32_t donate_p;
	uint32_t donate_v;
};

/*
 * Retrieves and returns hearts from varying locations and returns as a u32 int.
 * `save` - save file struct.
 * `offset` - offset to start reading from in save file.
 */
uint32_t _retrieve_hearts(struct SaveFile *save, D_OFFSET offset);


/*
 * Retrieves save file hearts and stores them in `SaveFile struct` passed to
 * function.
 * `save` - save file struct to be saved into.
 */
void fetch_savefile_hearts(struct SaveFile *save);

/*
 * Initializes save file pointers to `NULL` and integers to zero.
 * `save` - pointer to save file struct to be initialized.
 * `path` - string representing path to save file.
 */
void savefile_init(struct SaveFile *save, char *path);


/*
 * Destroys all remaining save file struct data.
 * `save` - pointer to save file struct to be destroyed.
 */
void destroy_savefile(struct SaveFile *save);

