#include <string.h>

#include "unity.h"

/* container */
#include "Vec.h"

/* lexer */
#include "Rational.h"
#include "Complex.h"
#include "Vector.h"
#include "Matrix.h"



void setUp(void) {
	initRational();
}

void tearDown(void) {
}

void test_ctor(void) {
	mpz_t	num;
	mpz_t	den;

	mpz_init_set_si(num, (long)-3);
	mpz_init_set_si(den, 5);
	struct s_Rational	*rational = new(Rational, RATIONAL, num, den);
	TEST_ASSERT_EQUAL(mpz_get_si(rational->numerator), -3);
	TEST_ASSERT_EQUAL(mpz_get_si(rational->denominator), 5);
	delete(rational);
	mpz_clear(num);
	mpz_clear(den);
}

void test_from_double1(void) {
	struct s_Rational	*rational = Rational_from_double(0.5);
	TEST_ASSERT_EQUAL(mpz_get_si(rational->numerator), 1);
	TEST_ASSERT_EQUAL(mpz_get_si(rational->denominator), 2);
	delete(rational);
}

void test_from_double2(void) {
	struct s_Rational	*rational = Rational_from_double(0.25);
	TEST_ASSERT_EQUAL(mpz_get_si(rational->numerator), 1);
	TEST_ASSERT_EQUAL(mpz_get_si(rational->denominator), 4);
	delete(rational);
}

void test_from_double3(void) {
	double				pi = 3.14159265358979323846;
	struct s_Rational	*rational = Rational_from_double(pi);
	double				val = Rational_to_double(rational);
	TEST_ASSERT_FLOAT_WITHIN(1e-15, pi, val);
	delete(rational);
}

void test_from_double4(void) {
	double 				target = -1.4142135623730951;
	struct s_Rational	*rational = Rational_from_double(target);
	double 				val = Rational_to_double(rational);
	TEST_ASSERT_FLOAT_WITHIN(1e-15, target, val);
	delete(rational);
}

void test_to_double(void) {
	struct s_Rational	*rational = Rational_from_long(1, (long)-1);
	double				val = Rational_to_double(rational);
	double				target = -0.1;
	TEST_ASSERT_FLOAT_WITHIN(1e-15, target, val);
	delete(rational);
}

void test_str(void) {
	double				val = -1.4142135623730951;
	char				*target = "-1.4142135623730951";
	struct s_Rational	*rational = Rational_from_double(val);
	const char			*s = str(rational);
	TEST_ASSERT_EQUAL_STRING_LEN(target, s, 7);
	delete(rational);
	free((void *)s);
}

void test_add(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(3, 5);
	struct s_Rational	*target = numeric_add(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->numerator), 11);
	TEST_ASSERT_EQUAL(mpz_get_si(target->denominator), 10);
	delete(a);
	delete(b);
	delete(target);
}

void test_sub(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(3, 5);
	struct s_Rational	*target = numeric_sub(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->numerator), -1);
	TEST_ASSERT_EQUAL(mpz_get_si(target->denominator), 10);
	delete(a);
	delete(b);
	delete(target);
}

void test_mul(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long(3, 5);
	struct s_Rational	*target = numeric_mul(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->numerator), 66);
	TEST_ASSERT_EQUAL(mpz_get_si(target->denominator), 35);
	delete(a);
	delete(b);
	delete(target);
}

void test_div(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long((long)-3, 5);
	struct s_Rational	*target = numeric_div(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->numerator), -110);
	TEST_ASSERT_EQUAL(mpz_get_si(target->denominator), 21);
	delete(a);
	delete(b);
	delete(target);
}

void test_mod1(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long(3, 5);
	struct s_Rational	*target = numeric_mod(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->numerator), 1);
	TEST_ASSERT_EQUAL(mpz_get_si(target->denominator), 7);
	delete(a);
	delete(b);
	delete(target);
}

void test_mod2(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long((long)-3, 5);
	struct s_Rational	*target = numeric_mod(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->numerator), -16);
	TEST_ASSERT_EQUAL(mpz_get_si(target->denominator), 35);
	delete(a);
	delete(b);
	delete(target);
}

void test_neg(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*target = numeric_neg(a);

	TEST_ASSERT_EQUAL(mpz_get_si(target->numerator), -1);
	TEST_ASSERT_EQUAL(mpz_get_si(target->denominator), 2);
	delete(a);
	delete(target);
}

void test_pow(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long(3, 5);
	struct s_Rational	*target = numeric_pow(a, b);
	double				val = Rational_to_double(target);
	TEST_ASSERT_FLOAT_WITHIN(1e-15, 1.9879011478604334, val);
	delete(a);
	delete(b);
	delete(target);
}

void test_pow_neg_to_non_integer(void) {
	struct s_Rational	*a = Rational_from_long((long)-1, 2);
	struct s_Rational	*b = Rational_from_long(21, 10);
	struct s_Complex	*target = numeric_pow(a, b);
	double				real = Rational_to_double(target->real);
	double				imag = Rational_to_double(target->imag);

	TEST_ASSERT_FLOAT_WITHIN(1e-15, 0.2218417766298604, real);
	TEST_ASSERT_FLOAT_WITHIN(1e-15, 0.07208076267434244, imag);
	delete(a);
	delete(b);
	delete(target);
}

void test_equal(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long(22, 7);
	struct s_Rational	*c = Rational_from_long(3, 5);

	TEST_ASSERT_TRUE(numeric_equal(a, b));
	TEST_ASSERT_FALSE(numeric_equal(a, c));
	delete(a);
	delete(b);
	delete(c);
}

void test_neq(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = Rational_from_long(22, 7);
	struct s_Rational	*c = Rational_from_long(3, 5);

	TEST_ASSERT_TRUE(Rational_neq(b, c));
	TEST_ASSERT_FALSE(Rational_neq(a, b));
	delete(a);
	delete(b);
	delete(c);
}

void test_lt(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(1, 3);

	TEST_ASSERT_TRUE(Rational_lt(b, a));
	TEST_ASSERT_FALSE(Rational_lt(a, b));
	TEST_ASSERT_FALSE(Rational_lt(a, a));
	delete(a);
	delete(b);
}

void test_gt(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(1, 3);

	TEST_ASSERT_TRUE(Rational_gt(a, b));
	TEST_ASSERT_FALSE(Rational_gt(b, a));
	TEST_ASSERT_FALSE(Rational_gt(a, a));
	delete(a);
	delete(b);
}

void test_le(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(1, 3);

	TEST_ASSERT_TRUE(Rational_le(b, a));
	TEST_ASSERT_FALSE(Rational_le(a, b));
	TEST_ASSERT_TRUE(Rational_le(a, a));
	delete(a);
	delete(b);
}

void test_ge(void) {
	struct s_Rational	*a = Rational_from_long(1, 2);
	struct s_Rational	*b = Rational_from_long(1, 3);

	TEST_ASSERT_TRUE(Rational_ge(a, b));
	TEST_ASSERT_FALSE(Rational_ge(b, a));
	TEST_ASSERT_TRUE(Rational_ge(a, a));
	delete(a);
	delete(b);
}

void test_promote_rational(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Rational	*b = numeric_promote(a, RATIONAL);

	TEST_ASSERT_TRUE(numeric_equal(a, b));
	delete(a);
	delete(b);
}

void test_promote_complex(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Complex	*b = numeric_promote(a, COMPLEX);
	struct s_Complex	*target = new(Complex, COMPLEX,
									Rational_from_long(22, 7),
									Rational_from_long(0, 1));

	TEST_ASSERT_TRUE(numeric_equal(b, target));
	delete(a);
	delete(b);
	delete(target);
}

void test_promote_vector(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Vector		*b = numeric_promote(a, VECTOR);
	struct s_Vector		*target = new(Vector, VECTOR, 1);

	Vector_update(target, 0, copy(a));
	TEST_ASSERT_TRUE(numeric_equal(b, target));
	delete(a);
	delete(b);
	delete(target);
}

void test_promote_matrix(void) {
	struct s_Rational	*a = Rational_from_long(22, 7);
	struct s_Matrix		*b = numeric_promote(a, MATRIX);
	void				*vec_v = new(Vec);
	void				*vec_m = new(Vec);
	struct s_Matrix		*target;

	Vec_push_back(vec_v, copy(a));
	Vec_push_back(vec_m, vec_v);
	target = new(Matrix, MATRIX, vec_m);
	TEST_ASSERT_TRUE(numeric_equal(b, target));
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
	RUN_TEST(test_str);
	RUN_TEST(test_add);
	RUN_TEST(test_sub);
	RUN_TEST(test_mul);
	RUN_TEST(test_div);
	RUN_TEST(test_mod1);
	RUN_TEST(test_mod2);
	RUN_TEST(test_neg);
	RUN_TEST(test_pow);
	RUN_TEST(test_pow_neg_to_non_integer);
	RUN_TEST(test_equal);
	RUN_TEST(test_neq);
	RUN_TEST(test_lt);
	RUN_TEST(test_gt);
	RUN_TEST(test_le);
	RUN_TEST(test_ge);
	RUN_TEST(test_promote_rational);
	RUN_TEST(test_promote_complex);
	RUN_TEST(test_promote_vector);
	RUN_TEST(test_promote_matrix);
	return UNITY_END();
}
