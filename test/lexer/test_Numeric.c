#include <string.h>

#include "unity.h"
#include "Rational.h"
#include "Complex.h"

void setUp(void) {
	initRational();
	initComplex();
}

void tearDown(void) {
}

void test_iadd(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(1, 1);
	numeric_iadd((void **)&a, a);
	TEST_ASSERT_EQUAL(mpz_get_si(a->numerator), mpz_get_si(b->numerator));
	TEST_ASSERT_EQUAL(mpz_get_si(a->denominator), mpz_get_si(b->denominator));
	delete(a);
	delete(b);
}

void test_isub(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(0, 1);
	numeric_isub((void **)&a, a);
	TEST_ASSERT_EQUAL(mpz_get_si(a->numerator), mpz_get_si(b->numerator));
	TEST_ASSERT_EQUAL(mpz_get_si(a->denominator), mpz_get_si(b->denominator));
	delete(a);
	delete(b);
}

void test_imul(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(1, 4);
	numeric_imul((void **)&a, a);
	TEST_ASSERT_EQUAL(mpz_get_si(a->numerator), mpz_get_si(b->numerator));
	TEST_ASSERT_EQUAL(mpz_get_si(a->denominator), mpz_get_si(b->denominator));
	delete(a);
	delete(b);
}

void test_idiv(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(1, 1);
	numeric_idiv((void **)&a, a);
	TEST_ASSERT_EQUAL(mpz_get_si(a->numerator), mpz_get_si(b->numerator));
	TEST_ASSERT_EQUAL(mpz_get_si(a->denominator), mpz_get_si(b->denominator));
	delete(a);
	delete(b);
}

void test_imod(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long(0, 1);
	numeric_imod((void **)&a, a);
	TEST_ASSERT_EQUAL(mpz_get_si(a->numerator), mpz_get_si(b->numerator));
	TEST_ASSERT_EQUAL(mpz_get_si(a->denominator), mpz_get_si(b->denominator));
	delete(a);
	delete(b);
}

void test_ineg(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long((long)-22, 7);;
	numeric_ineg((void **)&a);
	TEST_ASSERT_EQUAL(mpz_get_si(a->numerator), mpz_get_si(b->numerator));
	TEST_ASSERT_EQUAL(mpz_get_si(a->denominator), mpz_get_si(b->denominator));
	delete(a);
	delete(b);
}

void test_ipow(void) {
	struct s_Rational	*a = Rational_from_long(3, 1);
	struct s_Rational	*b = Rational_from_long(27, 1);
	numeric_ipow((void **)&a, a);
	TEST_ASSERT_EQUAL(mpz_get_si(a->numerator), mpz_get_si(b->numerator));
	TEST_ASSERT_EQUAL(mpz_get_si(a->denominator), mpz_get_si(b->denominator));
	delete(a);
	delete(b);
}

void test_ipromote(void) {
	struct s_Rational	*a = Rational_from_long(3, 1);
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long(3, 1),
								Rational_from_long(0, 1));
	numeric_ipromote((void **)&a, COMPLEX);
	TEST_ASSERT_TRUE(equal(a, b));
	delete(a);
	delete(b);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_iadd);
	RUN_TEST(test_isub);
	RUN_TEST(test_imul);
	RUN_TEST(test_idiv);
	RUN_TEST(test_imod);
	RUN_TEST(test_ineg);
	RUN_TEST(test_ipow);
	RUN_TEST(test_ipromote);
	return UNITY_END();
}
