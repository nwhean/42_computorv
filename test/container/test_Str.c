#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"
#include "Str.h"

void setUp(void) {
	initStr();
}

void tearDown(void) {
}

void test_swap(void) {
	void	*str1 = new(Str, "Hello World!");
	void	*str2 = new(Str, "Goodbye World!");
	char	*s1;
	char	*s2;

	swap_Str(str1, str2);
	s1 = str(str1);
	s2 = str(str2);
	TEST_ASSERT_EQUAL_STRING(s1, "Goodbye World!");
	TEST_ASSERT_EQUAL_STRING(s2, "Hello World!");
	delete(str1);
	delete(str2);
	free(s1);
	free(s2);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_swap);
	return UNITY_END();
}
