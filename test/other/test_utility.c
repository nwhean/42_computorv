#include <string.h>

#include "unity.h"
#include "utility.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_swap_size_t(void) {
	size_t	x = 1;
	size_t	y = 2;

	swap_size_t(&x, &y);
	TEST_ASSERT_EQUAL(x, 2);
	TEST_ASSERT_EQUAL(y, 1);
}

void test_swap_ptr(void) {
	size_t	a = 1;
	size_t	b = 2;
	size_t	*x = &a;
	size_t	*y = &b;

	swap_ptr((void **)&x, (void **)&y);
	TEST_ASSERT_EQUAL(*x, 2);
	TEST_ASSERT_EQUAL(*y, 1);
}


int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_swap_size_t);
	RUN_TEST(test_swap_ptr);
	return UNITY_END();
}
