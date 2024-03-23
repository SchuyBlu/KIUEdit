#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>
#include <3ds.h>
#include "arrays.h"


/*
 * Handles errors when opening archive.
 * `res` - Result of opening archive.
 * `name` - Name of archive being unmounted.
 */
void archive_handle_errors(Result res, const char *name);


/*
 * Loads archive and file for a game.
 * `name` - Name of archive being opened.
 */
void archive_open(const char *name);


/*
 * Closes an archive.
 * `name` - Name of archive being closed.
 */
void archive_close(const char *name);

/*
 * Gets a list of strings representing the directory listings.
 * `name` - Name of archive the directory is within.
 */
struct String_Array directory_strings(const char *name);

