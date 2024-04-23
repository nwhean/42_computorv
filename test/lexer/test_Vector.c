#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"
#include "Numeric.h"
#include "Rational.h"
#include "Vector.h"

void setUp(void) {
	initRational();
	initVector();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Vector		*v = new(Vector, VECTOR, 3, r0, r1, r2);
	TEST_ASSERT_TRUE(numeric_equal(r0, v->nums[0]));
	TEST_ASSERT_TRUE(numeric_equal(r1, v->nums[1]));
	TEST_ASSERT_TRUE(numeric_equal(r2, v->nums[2]));
	delete(v);
}

void test_str(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Vector		*v = new(Vector, VECTOR, 3, r0, r1, r2);
	char				*s = str(v);
	char				*target = "[0.500000, 0.750000, 0.833333]";
	TEST_ASSERT_EQUAL_STRING_LEN(target, s, 30);
	free((void *)s);
	delete(v);
}

void test_add_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	TEST_ASSERT_NULL(numeric_add(v0, r));
	delete(r);
	delete(v0);
}

void test_add_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Vector		*v1 = numeric_add(v0, v0);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 2);
	struct s_Vector		*target = new(Vector, VECTOR, 2, r2, r3);
	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(v0);
	delete(v1);
	delete(target);
}

void test_sub_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	TEST_ASSERT_NULL(numeric_sub(v0, r));
	delete(r);
	delete(v0);
}

void test_sub_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Vector		*v1 = numeric_sub(v0, v0);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 0, 1);
	struct s_Vector		*target = new(Vector, VECTOR, 2, r2, r3);
	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(v0);
	delete(v1);
	delete(target);
}

void test_mul_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	struct s_Vector		*v1 = numeric_mul(v0, r);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 4);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 8);
	struct s_Vector		*target = new(Vector, VECTOR, 2, r2, r3);
	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(r);
	delete(v0);
	delete(v1);
	delete(target);
}

void test_mul_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Vector		*v1 = numeric_mul(v0, v0);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 4);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 9, 16);
	struct s_Vector		*target = new(Vector, VECTOR, 2, r2, r3);
	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(v0);
	delete(v1);
	delete(target);
}

void test_div_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	struct s_Vector		*v1 = numeric_div(v0, r);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 2);
	struct s_Vector		*target = new(Vector, VECTOR, 2, r2, r3);
	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(r);
	delete(v0);
	delete(v1);
	delete(target);
}

void test_div_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Vector		*v1 = numeric_div(v0, v0);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 1, 1);
	struct s_Vector		*target = new(Vector, VECTOR, 2, r2, r3);
	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(v0);
	delete(v1);
	delete(target);
}

void test_mod_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	struct s_Vector		*v1 = numeric_mod(v0, r);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 1, 4);
	struct s_Vector		*target = new(Vector, VECTOR, 2, r2, r3);
	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(r);
	delete(v0);
	delete(v1);
	delete(target);
}

void test_mod_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	TEST_ASSERT_NULL(numeric_mod(v0, v0));
	delete(v0);
}

void test_neg(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r2 = new(Rational, RATIONAL, (long)-1, 2);
	struct s_Rational	*r3 = new(Rational, RATIONAL, (long)-3, 4);
	struct s_Vector		*v1 = new(Vector, VECTOR, 2, r2, r3);
	struct s_Vector		*target = numeric_neg(v0);

	TEST_ASSERT_TRUE(numeric_equal(v1, target));
	delete(v0);
	delete(v1);
	delete(target);
}

void test_pow(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	TEST_ASSERT_NULL(numeric_pow(v0, v0));
	delete(v0);
}

void test_equal_true(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v1 = new(Vector, VECTOR, 2, r2, r3);

	TEST_ASSERT_TRUE(numeric_equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_equal_false(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v0 = new(Vector, VECTOR, 2, r0, r1);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 3);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 4);
	struct s_Vector		*v1 = new(Vector, VECTOR, 2, r2, r3);

	TEST_ASSERT_FALSE(numeric_equal(v0, v1));
	delete(v0);
	delete(v1);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_ctor);
	RUN_TEST(test_str);
	RUN_TEST(test_add_scalar);
	RUN_TEST(test_add_vector);
	RUN_TEST(test_sub_scalar);
	RUN_TEST(test_sub_vector);
	RUN_TEST(test_mul_scalar);
	RUN_TEST(test_mul_vector);
	RUN_TEST(test_div_scalar);
	RUN_TEST(test_div_vector);
	RUN_TEST(test_mod_scalar);
	RUN_TEST(test_mod_vector);
	RUN_TEST(test_neg);
	RUN_TEST(test_pow);
	RUN_TEST(test_equal_true);
	RUN_TEST(test_equal_false);
	return UNITY_END();
}
