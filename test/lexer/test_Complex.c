#include <string.h>

#include "unity.h"
#include "Vec.h"
#include "Rational.h"
#include "Complex.h"
#include "Vector.h"
#include "Matrix.h"

void setUp(void) {
	initComplex();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Complex	*comp = new(Complex, COMPLEX,
									Rational_from_long((long)22, (long)7),
									Rational_from_long((long)-3, (long)5)
									);
	TEST_ASSERT_EQUAL(mpz_get_si(comp->real->numerator), 22);
	TEST_ASSERT_EQUAL(mpz_get_si(comp->real->denominator), 7);
	TEST_ASSERT_EQUAL(mpz_get_si(comp->imag->numerator), (long)-3);
	TEST_ASSERT_EQUAL(mpz_get_si(comp->imag->denominator), 5);
	delete(comp);
}

void test_str(void) {
	struct s_Complex	*comp = new(Complex, COMPLEX,
									Rational_from_long((long)1, (long)1),
									Rational_from_long((long)2, (long)1)
									);
	const char			*s = str(comp);
	const char			*target = "1+2i";
	TEST_ASSERT_EQUAL_STRING_LEN(target, s, 7);
	delete(comp);
	free((void *)s);
}

void test_add(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long((long)5, (long)6),
								Rational_from_long((long)7, (long)8));
	struct s_Complex	*target = numeric_add(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->numerator), 4);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->denominator), 3);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->numerator), 13);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->denominator), 8);
	delete(a);
	delete(b);
	delete(target);
}

void test_sub(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long((long)5, (long)6),
								Rational_from_long((long)7, (long)8));
	struct s_Complex	*target = numeric_sub(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->numerator), -1);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->denominator), 3);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->numerator), -1);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->denominator), 8);
	delete(a);
	delete(b);
	delete(target);
}

void test_mul(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long((long)5, (long)6),
								Rational_from_long((long)7, (long)8));
	struct s_Complex	*target = numeric_mul(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->numerator), -23);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->denominator), 96);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->numerator), 17);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->denominator), 16);
	delete(a);
	delete(b);
	delete(target);
}

void test_div(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long((long)5, (long)6),
								Rational_from_long((long)7, (long)8));
	struct s_Complex	*target = numeric_div(a, b);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->numerator), 618);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->denominator), 841);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->numerator), 108);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->denominator), 841);
	delete(a);
	delete(b);
	delete(target);
}

void test_neg(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*target = numeric_neg(a);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->numerator), -1);
	TEST_ASSERT_EQUAL(mpz_get_si(target->real->denominator), 2);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->numerator), -3);
	TEST_ASSERT_EQUAL(mpz_get_si(target->imag->denominator), 4);
	delete(a);
	delete(target);
}

void test_pow_rational(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Rational	*b = Rational_from_long((long)5, (long)6);
	struct s_Complex	*target = numeric_pow(a, b);
	double				real = Rational_to_double(target->real);
	double				imag = Rational_to_double(target->imag);

	TEST_ASSERT_FLOAT_WITHIN(1e-15, 0.626352798861, real);
	TEST_ASSERT_FLOAT_WITHIN(1e-15, 0.669919542924, imag);
	delete(a);
	delete(b);
	delete(target);
}

void test_pow_complex(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long((long)5, (long)6),
								Rational_from_long((long)7, (long)8));
	struct s_Complex	*target = numeric_pow(a, b);
	double				real = Rational_to_double(target->real);
	double				imag = Rational_to_double(target->imag);
	TEST_ASSERT_FLOAT_WITHIN(1e-15, 0.289688910906, real);
	TEST_ASSERT_FLOAT_WITHIN(1e-15, 0.258285468769, imag);
	delete(a);
	delete(b);
	delete(target);
}

void test_equal_true(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));

	TEST_ASSERT_TRUE(numeric_equal(a, b));
	delete(a);
	delete(b);
}

void test_equal_false(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long((long)1, (long)2),
								Rational_from_long((long)3, (long)4));
	struct s_Complex	*b = new(Complex, COMPLEX,
								Rational_from_long((long)5, (long)6),
								Rational_from_long((long)7, (long)8));

	TEST_ASSERT_FALSE(numeric_equal(a, b));
	delete(a);
	delete(b);
}

void test_promote_rational(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	struct s_Rational	*b = numeric_promote(a, RATIONAL);

	TEST_ASSERT_NULL(b);
	delete(a);
}

void test_promote_complex(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	struct s_Complex	*b = numeric_promote(a, COMPLEX);

	TEST_ASSERT_TRUE(numeric_equal(b, a));
	delete(a);
	delete(b);
}

void test_promote_vector(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	struct s_Vector		*b = numeric_promote(a, VECTOR);
	struct s_Vector		*target = new(Vector, VECTOR, 1);

	Vector_update(target, 0, copy(a));
	TEST_ASSERT_TRUE(numeric_equal(b, target));
	delete(a);
	delete(b);
	delete(target);
}

void test_promote_matrix(void) {
	struct s_Complex	*a = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
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
	RUN_TEST(test_str);
	RUN_TEST(test_add);
	RUN_TEST(test_sub);
	RUN_TEST(test_mul);
	RUN_TEST(test_div);
	RUN_TEST(test_neg);
	RUN_TEST(test_pow_rational);
	RUN_TEST(test_pow_complex);
	RUN_TEST(test_equal_true);
	RUN_TEST(test_equal_false);
	RUN_TEST(test_promote_rational);
	RUN_TEST(test_promote_complex);
	RUN_TEST(test_promote_vector);
	RUN_TEST(test_promote_matrix);
	return UNITY_END();
}
