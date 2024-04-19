#include <string.h>

#include "unity.h"
#include "Rational.h"
#include "Complex.h"

void setUp(void) {
	initComplex();
	initRational();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Complex	*comp = new(Complex, COMPLEX,
									new(Rational, RATIONAL, (long)22, (long)7),
									new(Rational, RATIONAL, (long)-3, (long)5)
									);
	TEST_ASSERT_EQUAL(comp->real->numerator, 22);
	TEST_ASSERT_EQUAL(comp->real->denominator, 7);
	TEST_ASSERT_EQUAL(comp->imag->numerator, -3);
	TEST_ASSERT_EQUAL(comp->imag->denominator, 5);
	delete(comp);
}

void test_to_string(void) {
	struct s_Complex	*comp = new(Complex, COMPLEX,
									new(Rational, RATIONAL, (long)1, (long)1),
									new(Rational, RATIONAL, (long)2, (long)1)
									);
	const char			*str = token_to_string(comp);
	const char			*target = "1+2i";
	TEST_ASSERT_EQUAL_STRING_LEN(target, str, 7);
	delete(comp);
	free((void *)str);
}

void test_add(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)1, (long)2),
								new(Rational, RATIONAL, (long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)5, (long)6),
								new(Rational, RATIONAL, (long)7, (long)8));
	struct s_Complex	*target = numeric_add(a, b);
	TEST_ASSERT_EQUAL(target->real->numerator, 4);
	TEST_ASSERT_EQUAL(target->real->denominator, 3);
	TEST_ASSERT_EQUAL(target->imag->numerator, 13);
	TEST_ASSERT_EQUAL(target->imag->denominator, 8);
	delete(a);
	delete(b);
	delete(target);
}

void test_sub(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)1, (long)2),
								new(Rational, RATIONAL, (long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)5, (long)6),
								new(Rational, RATIONAL, (long)7, (long)8));
	struct s_Complex	*target = numeric_sub(a, b);
	TEST_ASSERT_EQUAL(target->real->numerator, -1);
	TEST_ASSERT_EQUAL(target->real->denominator, 3);
	TEST_ASSERT_EQUAL(target->imag->numerator, -1);
	TEST_ASSERT_EQUAL(target->imag->denominator, 8);
	delete(a);
	delete(b);
	delete(target);
}

void test_mul(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)1, (long)2),
								new(Rational, RATIONAL, (long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)5, (long)6),
								new(Rational, RATIONAL, (long)7, (long)8));
	struct s_Complex	*target = numeric_mul(a, b);
	TEST_ASSERT_EQUAL(target->real->numerator, -23);
	TEST_ASSERT_EQUAL(target->real->denominator, 96);
	TEST_ASSERT_EQUAL(target->imag->numerator, 17);
	TEST_ASSERT_EQUAL(target->imag->denominator, 16);
	delete(a);
	delete(b);
	delete(target);
}

void test_div(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)1, (long)2),
								new(Rational, RATIONAL, (long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)5, (long)6),
								new(Rational, RATIONAL, (long)7, (long)8));
	struct s_Complex	*target = numeric_div(a, b);
	TEST_ASSERT_EQUAL(target->real->numerator, 618);
	TEST_ASSERT_EQUAL(target->real->denominator, 841);
	TEST_ASSERT_EQUAL(target->imag->numerator, 108);
	TEST_ASSERT_EQUAL(target->imag->denominator, 841);
	delete(a);
	delete(b);
	delete(target);
}

void test_neg(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)1, (long)2),
								new(Rational, RATIONAL, (long)3, (long)4));
	struct s_Complex	*target = numeric_neg(a);
	TEST_ASSERT_EQUAL(target->real->numerator, -1);
	TEST_ASSERT_EQUAL(target->real->denominator, 2);
	TEST_ASSERT_EQUAL(target->imag->numerator, -3);
	TEST_ASSERT_EQUAL(target->imag->denominator, 4);
	delete(a);
	delete(target);
}

void test_pow_rational(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)1, (long)2),
								new(Rational, RATIONAL, (long)3, (long)4));
	struct s_Rational	*b = new(Rational, RATIONAL, (long)5, (long)6);
	struct s_Complex	*target = numeric_pow(a, b);
	double				real = Rational_to_double(target->real);
	double				imag = Rational_to_double(target->imag);

	TEST_ASSERT_FLOAT_WITHIN(1e-7, real, 0.626352798861);
	TEST_ASSERT_FLOAT_WITHIN(1e-7, imag, 0.669919542924);
	delete(a);
	delete(b);
	delete(target);
}

void test_pow_complex(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)1, (long)2),
								new(Rational, RATIONAL, (long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								new(Rational, RATIONAL, (long)5, (long)6),
								new(Rational, RATIONAL, (long)7, (long)8));
	struct s_Complex	*target = numeric_pow(a, b);
	double				real = Rational_to_double(target->real);
	double				imag = Rational_to_double(target->imag);
	TEST_ASSERT_FLOAT_WITHIN(1e-7, real, 0.289688910906);
	TEST_ASSERT_FLOAT_WITHIN(1e-7, imag, 0.258285468769);
	delete(a);
	delete(b);
	delete(target);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_ctor);
	RUN_TEST(test_to_string);
	RUN_TEST(test_add);
	RUN_TEST(test_sub);
	RUN_TEST(test_mul);
	RUN_TEST(test_div);
	RUN_TEST(test_neg);
	RUN_TEST(test_pow_rational);
	RUN_TEST(test_pow_complex);
	return UNITY_END();
}
