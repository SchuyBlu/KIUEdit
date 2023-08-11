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


void savefile_init(struct SaveFile *save, char *path)
{
        // Initialize all values.
        save->fp = NULL;
        save->hearts = 0;
        save->donate_p = 0;
        save->donate_v = 0;

        // Attempt to open save file.
        save->fp = fopen(path, "r+");
        assert(save->fp);
}


void destroy_savefile(struct SaveFile *save)
{
        fclose(save->fp);
}


uint32_t _retrieve_hearts(struct SaveFile *save, int offset)
{
        // Set heart buffer to three because I'm only seeking out 3 bytes
        char heart_buf[3];
        uint32_t hearts;

        // Read three bytes after heart offset into hearts variable
        fseek(save->fp, offset, SEEK_SET);
        fread(heart_buf, 3, 1, save->fp);
        memcpy(&hearts, heart_buf, 3);

        return hearts;
}

