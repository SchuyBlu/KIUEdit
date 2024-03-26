#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <3ds.h>
#include "menu.h"
#include "archive.h"

int main(void)
{
	// -----------------------------------------------------
	// Build each submenu
	Menu menu;
	Submenu *root, *businesses, *schools, *cities;

	// Sets up Menu struct that contains the root node,
	// and initializes the root submenu of the menu system.
	menu_init(&menu, "Menu System", switch_submenu);
	set_submenu_desc((root = menu.curr), "Choose an option:");

	// Add a few options to the root page.
	schools = add_submenu_option(root, "Universities", switch_submenu);
	businesses = add_submenu_option(root, "Businesses", switch_submenu);
	cities = add_submenu_option(root, "Cities", switch_submenu);

	// Add a few options to the school page
	add_submenu_option(schools, "University of Alberta", switch_submenu);
	add_submenu_option(schools, "MacEwan University", switch_submenu);
	add_submenu_option(schools, "NAIT", switch_submenu);
	add_submenu_option(schools, "Norquest", switch_submenu);

	// Add a few to the businesses page
	add_submenu_option(businesses, "7-Eleven", switch_submenu);
	add_submenu_option(businesses, "Tim Hortons", switch_submenu);
	add_submenu_option(businesses, "Auto Repair", switch_submenu);
	add_submenu_option(businesses, "Safeway", switch_submenu);

	// Add a few to the cities page
	add_submenu_option(cities, "Edmonton", switch_submenu);
	add_submenu_option(cities, "Calgary", switch_submenu);
	add_submenu_option(cities, "Grande Prairie", switch_submenu);
	add_submenu_option(cities, "Vancouver", switch_submenu);
	// -----------------------------------------------------
	uint32_t k_down_old = 0;

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	// Enter main loop here
	print_menu(&menu, NULL);
	while (aptMainLoop()) {
		hidScanInput();

		uint32_t k_down = hidKeysDown();

		if (k_down & KEY_START) break;

		if (k_down != k_down_old) {
			consoleClear();
			print_menu(&menu, NULL);
		}

		move_menu_cursor(&menu, k_down);
		menu.curr->action(&menu, &k_down);

		k_down_old = k_down;

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	destroy_menu(&menu);

	gfxExit();

	return 0;
}

