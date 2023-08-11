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
#include "unity.h"

struct SaveFile save;


void setUp()
{
        savefile_init(&save, "test/input/in.sav");
}


void tearDown()
{
        destroy_savefile(&save);
}


void test_ifSaveIsInitializedProperlyThenFile_should_beTruthy(void)
{
        TEST_ASSERT_TRUE(save.fp);
}


void test_ifHeartsAreRead_should_returnProperHeartCount(void)
{
        uint32_t hearts;
        hearts = _retrieve_hearts(&save, HEART_OFFSET);
        TEST_ASSERT_EQUAL_UINT32(9853047, hearts);
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_ifSaveIsInitializedProperlyThenFile_should_beTruthy);
        RUN_TEST(test_ifHeartsAreRead_should_returnProperHeartCount);
        UNITY_END();
}

