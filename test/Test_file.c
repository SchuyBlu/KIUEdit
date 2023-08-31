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


void test_ifSaveIsInitializedProperly_should_haveAllProperHeartData(void)
{
        TEST_ASSERT_EQUAL_UINT32(9853047, save.hearts);
        TEST_ASSERT_EQUAL_UINT32(9999999, save.donate_p);
        TEST_ASSERT_EQUAL_UINT32(0, save.donate_v);
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_ifSaveIsInitializedProperlyThenFile_should_beTruthy);
        RUN_TEST(test_ifHeartsAreRead_should_returnProperHeartCount);
        RUN_TEST(test_ifSaveIsInitializedProperly_should_haveAllProperHeartData);
        UNITY_END();
}

