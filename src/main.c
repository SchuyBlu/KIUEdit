#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <3ds.h>
#include "pages.h"
#include "archive.h"

int main(void)
{
	// -----------------------------------------------------
	// Build each page
	struct Pages pages;
	struct Page *root, *businesses, *schools, *cities;

	// Sets up Pages struct that contains the root node,
	// and initializes the root page of the menu system.
	pages_init(&pages, "Menu System", switch_page);
	set_page_desc((root = pages.curr), "Choose an option:");

	// Add a few options to the root page.
	schools = add_page_option(root, "Universities", switch_page);
	businesses = add_page_option(root, "Businesses", switch_page);
	cities = add_page_option(root, "Cities", switch_page);

	// Add a few options to the school page
	add_page_option(schools, "University of Alberta", switch_page);
	add_page_option(schools, "MacEwan University", switch_page);
	add_page_option(schools, "NAIT", switch_page);
	add_page_option(schools, "Norquest", switch_page);

	// Add a few to the businesses page
	add_page_option(businesses, "7-Eleven", switch_page);
	add_page_option(businesses, "Tim Hortons", switch_page);
	add_page_option(businesses, "Auto Repair", switch_page);
	add_page_option(businesses, "Safeway", switch_page);

	// Add a few to the cities page
	add_page_option(cities, "Edmonton", switch_page);
	add_page_option(cities, "Calgary", switch_page);
	add_page_option(cities, "Grande Prairie", switch_page);
	add_page_option(cities, "Vancouver", switch_page);
	// -----------------------------------------------------
	uint32_t k_down_old = 0;

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	// Enter main loop here
	print_page(&pages, NULL);
	while (aptMainLoop()) {
		hidScanInput();

		uint32_t k_down = hidKeysDown();

		if (k_down & KEY_START) break;

		if (k_down != k_down_old) {
			consoleClear();
			print_page(&pages, NULL);
		}

		move_page_cursor(&pages, k_down);
		pages.curr->action(&pages, &k_down);

		k_down_old = k_down;

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	destroy_pages(&pages);

	gfxExit();

	return 0;
}

