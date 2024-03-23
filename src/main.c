#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <3ds.h>
#include "pages.h"
#include "archive.h"

int main(void)
{
	uint32_t k_down_old = 0;

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	struct Pages pages;

	pages_init(&pages, "Kid Icarus Uprising Save Editor", MISC_PAGE);
	set_page_desc(pages.curr, "Choose a save to edit:");

	archive_open("KIU");

	struct String_Array strings = directory_strings("KIU");
	for (int i = 0; i < strings.len; i++) {
		add_page_option(pages.curr, strings.list[i], MISC_PAGE);
	}
	strings_destroy(&strings);

	// Enter main loop here
	print_page(&pages, NULL);
	while (aptMainLoop()) {
		hidScanInput();

		uint32_t k_down = hidKeysDown();

		if (k_down & KEY_START) break;

		if (k_down != k_down_old) {
			consoleClear();
			move_page_cursor(&pages, k_down);
			print_page(&pages, NULL);
		}

		k_down_old = k_down;

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	destroy_pages(&pages);

	archive_close("KIU");
	gfxExit();

	return 0;
}

