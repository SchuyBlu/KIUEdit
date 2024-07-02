#include "menub.h"
#include "unity.h"


void setUp()
{
	return;
}


void tearDown()
{
	return;
}


void test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectLength(void)
{
	int len = 0;
	char **list = get_save_strings("test/test_dirs/test_all", &len);

	TEST_ASSERT_EQUAL_INT(4, len);

	for(int i = 0; i < len; i++)
		free(list[i]);
	free(list);
}


void test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectFilenames(void)
{
	int len = 0;
	char **list = get_save_strings("test/test_dirs/test_all", &len);
	
	TEST_ASSERT_EQUAL_STRING("00.SAV", list[0]);
	TEST_ASSERT_EQUAL_STRING("01.SAV", list[1]);
	TEST_ASSERT_EQUAL_STRING("02.SAV", list[2]);
	TEST_ASSERT_EQUAL_STRING("03.SAV", list[3]);

	for (int i = 0; i < len; i++)
		free(list[i]);
	free(list);
}


void test_whenDirectoryIsLoadedWithOneFile_should_containCorrectLenAndName(void)
{
	int len = 0;
	char **list = get_save_strings("test/test_dirs/test_one", &len);

	TEST_ASSERT_EQUAL_INT(1, len);
	TEST_ASSERT_EQUAL_STRING("01.SAV", list[0]);

	for (int i = 0; i < len; i++)
		free(list[i]);
	free(list);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectLength);
	RUN_TEST(test_whenDirectoryIsLoadedWithThreeFiles_should_containCorrectFilenames);
	UNITY_END();
}

