// MIT License
//
// Copyright (c) 2023 Schuy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "file.h"


uint32_t _retrieve_hearts(struct SaveFile *save, D_OFFSET offset)
{
        // Set heart buffer to three because I'm only seeking out 3 bytes
		uint32_t hearts = 0;

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

