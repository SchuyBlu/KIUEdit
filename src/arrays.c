#include "arrays.h"


void strings_init(struct String_Array *strings)
{
	strings->cap = 1;
	strings->len = 0;
	strings->list = malloc(sizeof(char*) * strings->cap);
	assert(strings->list);
}


void strings_append(struct String_Array *strings, char *string)
{
	// Change size if cap is length.
	if (strings->cap == strings->len) {
		strings->cap *= 2;
		strings->list = realloc(strings->list, sizeof(char*) * strings->cap);
		assert(strings->list);
	}

	strings->list[strings->len] = malloc(sizeof(char) * strlen(string) + 1);
	strcpy(strings->list[strings->len], string);
	assert(strings->list[strings->len]);

	strings->len++;
}


void strings_destroy(struct String_Array *strings)
{
	for (int i = 0; i < strings->len; i++) {
		free(strings->list[i]);
	}
}

