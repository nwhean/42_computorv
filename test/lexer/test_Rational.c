#include <string.h>

#include "unity.h"
#include "Rational.h"

void setUp(void) {
	initRational();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Rational	*rational = new(Rational, RATIONAL, (long)-3, (long)5);
	TEST_ASSERT_EQUAL(rational->numerator, -3);
	TEST_ASSERT_EQUAL(rational->denominator, 5);
	delete(rational);
}

void test_from_double1(void) {
	struct s_Rational	*rational = Rational_from_double(0.5);
	TEST_ASSERT_EQUAL(rational->numerator, 1);
	TEST_ASSERT_EQUAL(rational->denominator, 2);
	delete(rational);
}

void test_from_double2(void) {
	struct s_Rational	*rational = Rational_from_double(0.1);
	TEST_ASSERT_EQUAL(rational->numerator, 1);
	TEST_ASSERT_EQUAL(rational->denominator, 10);
	delete(rational);
}

void test_from_double3(void) {
	double				pi = 3.14159265358979323846;
	struct s_Rational	*rational = Rational_from_double(pi);
	double				val = Rational_to_double(rational);
	TEST_ASSERT_FLOAT_WITHIN(1e-7, pi, val);
	delete(rational);
}

void test_from_double4(void) {
	double 				target = -1.4142135623730951;
	struct s_Rational	*rational = Rational_from_double(target);
	double 				val = Rational_to_double(rational);
	TEST_ASSERT_FLOAT_WITHIN(1e-7, target, val);
	delete(rational);
}

void test_to_double(void) {
	struct s_Rational	*rational = new(Rational, RATIONAL, (long)1, (long)-1);
	double				val = Rational_to_double(rational);
	double				target = -0.1;
	TEST_ASSERT_FLOAT_WITHIN(1e-7, target, val);
	delete(rational);
}

void test_to_string(void) {
	double				val = -1.4142135623730951;
	char				*target = "-1.4142135623730951";
	struct s_Rational	*rational = Rational_from_double(val);
	const char			*str = token_to_string(rational);
	TEST_ASSERT_EQUAL_STRING_LEN(target, str, 7);
	delete(rational);
	free((void *)str);
}

void test_add(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)1, (long)2);
	struct s_Rational	*b = new(Rational, RATIONAL, (long)3, (long)5);
	struct s_Rational	*target = numeric_add(a, b);
	TEST_ASSERT_EQUAL(target->numerator, 11);
	TEST_ASSERT_EQUAL(target->denominator, 10);
	delete(a);
	delete(b);
	delete(target);
}

void test_sub(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)1, (long)2);
	struct s_Rational	*b = new(Rational, RATIONAL, (long)3, (long)5);
	struct s_Rational	*target = numeric_sub(a, b);
	TEST_ASSERT_EQUAL(target->numerator, -1);
	TEST_ASSERT_EQUAL(target->denominator, 10);
	delete(a);
	delete(b);
	delete(target);
}

void test_mul(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)22, (long)7);
	struct s_Rational	*b = new(Rational, RATIONAL, (long)3, (long)5);
	struct s_Rational	*target = numeric_mul(a, b);
	TEST_ASSERT_EQUAL(target->numerator, 66);
	TEST_ASSERT_EQUAL(target->denominator, 35);
	delete(a);
	delete(b);
	delete(target);
}

void test_div(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)22, (long)7);
	struct s_Rational	*b = new(Rational, RATIONAL, (long)-3, (long)5);
	struct s_Rational	*target = numeric_div(a, b);
	TEST_ASSERT_EQUAL(target->numerator, -110);
	TEST_ASSERT_EQUAL(target->denominator, 21);
	delete(a);
	delete(b);
	delete(target);
}

void test_mod1(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)22, (long)7);
	struct s_Rational	*b = new(Rational, RATIONAL, (long)3, (long)5);
	struct s_Rational	*target = numeric_mod(a, b);
	TEST_ASSERT_EQUAL(target->numerator, 1);
	TEST_ASSERT_EQUAL(target->denominator, 7);
	delete(a);
	delete(b);
	delete(target);
}

void test_mod2(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)22, (long)7);
	struct s_Rational	*b = new(Rational, RATIONAL, (long)-3, (long)5);
	struct s_Rational	*target = numeric_mod(a, b);
	TEST_ASSERT_EQUAL(target->numerator, -16);
	TEST_ASSERT_EQUAL(target->denominator, 35);
	delete(a);
	delete(b);
	delete(target);
}

void test_neg(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)1, (long)2);
	struct s_Rational	*target = numeric_neg(a);

	TEST_ASSERT_EQUAL(target->numerator, -1);
	TEST_ASSERT_EQUAL(target->denominator, 2);
	delete(a);
	delete(target);
}

void test_pow(void) {
	struct s_Rational	*a = new(Rational, RATIONAL, (long)22, (long)7);
	struct s_Rational	*b = new(Rational, RATIONAL, (long)3, (long)5);
	struct s_Rational	*target = numeric_pow(a, b);
	double				val = Rational_to_double(target);
	TEST_ASSERT_FLOAT_WITHIN(1e-7, val, 1.9879011478604334);
	delete(a);
	delete(b);
	delete(target);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_ctor);
	RUN_TEST(test_from_double1);
	RUN_TEST(test_from_double2);
	RUN_TEST(test_from_double3);
	RUN_TEST(test_from_double4);
	RUN_TEST(test_to_string);
	RUN_TEST(test_add);
	RUN_TEST(test_sub);
	RUN_TEST(test_mul);
	RUN_TEST(test_div);
	RUN_TEST(test_mod1);
	RUN_TEST(test_mod2);
	RUN_TEST(test_neg);
	RUN_TEST(test_pow);
	return UNITY_END();
}
