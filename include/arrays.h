/*
 * Author: Schuy
 * Simple collection of functions related to dynamic arrays.
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

struct String_Array {
	char **list;
	uint32_t cap;
	uint32_t len;
};


/*
 * Initialize a string array.
 * `strings` - pointer to a String_Array struct.
 */
void strings_init(struct String_Array *strings);


/*
 * Appends a string to an array of strings.
 * `strings` - pointer to a String_Array struct.
 * `string` - string being appended.
 */
void strings_append(struct String_Array *strings, char *string);


/*
 * Destroys a list of strings.
 * `strings` pointer to a StringArray struct.
 */
void strings_destroy(struct String_Array *strings);
