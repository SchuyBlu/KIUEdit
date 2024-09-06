#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <3ds.h>
#include "editor.h"
#include "archive.h"

int main(void)
{
	archive_open("KIU");
	Editor editor = editor_init();

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	// Enter main loop here
	print_menu(&editor.menu, NULL);
	while (aptMainLoop()) {
		hidScanInput();

		editor.ctx.key_down = hidKeysDown();

		if (editor.ctx.key_down & KEY_START) break;

		if (editor.ctx.key_down != editor.ctx.key_down_old) {
			consoleClear();
			print_menu(&editor.menu, NULL);
		}

		move_menu_cursor(&editor.menu, editor.ctx.key_down);
		editor.menu.curr->action(&editor.menu, &editor.ctx);

		editor.ctx.key_down_old = editor.ctx.key_down;

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	archive_close("KIU");
	destroy_menu(&editor.menu);

	gfxExit();

	return 0;
}

