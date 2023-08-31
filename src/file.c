#include "file.h"


uint32_t _retrieve_hearts(struct SaveFile *save, D_OFFSET offset)
{
        // Set heart buffer to three because I'm only seeking out 3 bytes
		uint32_t hearts;

        // Read three bytes after heart offset into hearts variable
        fseek(save->fp, offset, SEEK_SET);
        fread(&hearts, 3, 1, save->fp);
		hearts &= 0xFFFFFF;

        return hearts;
}


void fetch_savefile_hearts(struct SaveFile *save)
{
        save->hearts = _retrieve_hearts(save, HEART_OFFSET);
        save->donate_p = _retrieve_hearts(save, PALU_OFFSET);
        save->donate_v = _retrieve_hearts(save, VIRIDI_OFFSET);
}


void savefile_init(struct SaveFile *save, char *path)
{
        save->fp = NULL;

        // Attempt to open save file.
        save->fp = fopen(path, "r+");
        assert(save->fp);

        fetch_savefile_hearts(save);
}


void destroy_savefile(struct SaveFile *save)
{
        fclose(save->fp);
}

