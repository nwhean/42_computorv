#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"

/* container */
#include "Vec.h"

/* lexer */
#include "Complex.h"
#include "Numeric.h"
#include "Polynomial.h"
#include "Rational.h"

void setUp(void) {
	initPolynomial();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(5, 6);
	struct s_Polynomial	*v;

	v = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v, 0, r0);
	Polynomial_update(v, 2, r2);
	Polynomial_update(v, 1, r1);
	TEST_ASSERT_TRUE(numeric_equal(r0, Polynomial_at(v, 0)));
	TEST_ASSERT_TRUE(numeric_equal(r1, Polynomial_at(v, 1)));
	TEST_ASSERT_TRUE(numeric_equal(r2, Polynomial_at(v, 2)));
	delete(v);
}

void test_str(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(5, 6);
	struct s_Polynomial	*v;
	char				*s;
	char				*target;

	v = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v, 0, r0);
	Polynomial_update(v, 1, r1);
	Polynomial_update(v, 2, r2);
	s = str(v);
	target = "Polynomial(, 0.500000, 0.750000, 0.833333)";
	TEST_ASSERT_EQUAL_STRING_LEN(target, s, 30);
	free((void *)s);
	delete(v);
}

void test_add_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r = Rational_from_long(1, 2);
	struct s_Rational	*target_0 = Rational_from_long(1, 1);
	struct s_Rational	*target_1 = copy(r1);
	struct s_Polynomial	*v;

	v = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v, 0, r0);
	Polynomial_update(v, 1, r1);
	numeric_iadd((void **)&v, r);
	TEST_ASSERT_TRUE(numeric_equal(Polynomial_at(v, 0), target_0));
	TEST_ASSERT_TRUE(numeric_equal(Polynomial_at(v, 1), target_1));
	delete(r);
	delete(target_0);
	delete(target_1);
	delete(v);
}

void test_add_polynomial(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 1);
	struct s_Rational	*r3 = Rational_from_long(3, 2);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v1;
	struct s_Polynomial		*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 0, r2);
	Polynomial_update(v1, 1, r3);

	v = numeric_add(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_sub_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r = Rational_from_long(1, 2);
	struct s_Rational	*x0 = Rational_from_long(0, 1);
	struct s_Rational	*x1 = Rational_from_long(3, 4);
	void				*v;

	v = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v, 0, r0);
	Polynomial_update(v, 1, r1);
	numeric_isub(&v, r);
	TEST_ASSERT_TRUE(equal(Polynomial_at(v, 0), x0));
	TEST_ASSERT_TRUE(equal(Polynomial_at(v, 1), x1));
	delete(r);
	delete(v);
	delete(x0);
	delete(x1);
}

void test_sub_polynomial(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v1;
	struct s_Polynomial		*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);

	v = numeric_sub(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_mul_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Polynomial		*v0;

	struct s_Rational	*r2 = Rational_from_long(7, 2);
	struct s_Rational	*r3 = Rational_from_long(21, 4);
	struct s_Polynomial		*v1;

	struct s_Rational	*r = Rational_from_long(7, 1);
	struct s_Polynomial		*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 0, r2);
	Polynomial_update(v1, 1, r3);

	v = numeric_mul(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_mul_polynomial(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v = numeric_mul(v0, v0);
	TEST_ASSERT_NULL(v);
	delete(v0);
}

void test_div_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 14);
	struct s_Rational	*r3 = Rational_from_long(3, 28);
	struct s_Rational	*r = Rational_from_long(7, 1);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v1;
	struct s_Polynomial		*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 0, r2);
	Polynomial_update(v1, 1, r3);

	v = numeric_div(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_div_polynomial(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v = numeric_div(v0, v0);
	TEST_ASSERT_NULL(v);
	delete(v0);
}

void test_mod_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(0, 1);
	struct s_Rational	*r3 = Rational_from_long(1, 4);
	struct s_Rational	*r = Rational_from_long(1, 2);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v1;
	struct s_Polynomial		*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 0, r2);
	Polynomial_update(v1, 1, r3);

	v = numeric_mod(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_mod_polynomial(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Polynomial		*v0;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	TEST_ASSERT_NULL(numeric_mod(v0, v0));
	delete(v0);
}

void test_neg(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long((long)-1, 2);
	struct s_Rational	*r3 = Rational_from_long((long)-3, 4);
	struct s_Polynomial	*v0;
	struct s_Polynomial	*v1;
	struct s_Polynomial	*v;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 0, r2);
	Polynomial_update(v1, 1, r3);

	v = numeric_neg(v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_pow(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r = Rational_from_long(3, 1);
	struct s_Rational	*coeff_0 = numeric_pow(r0, r);
	struct s_Rational	*coeff_1 = numeric_pow(r1, r);;
	struct s_Polynomial	*v0;
	struct s_Polynomial	*v1;
	struct s_Polynomial	*target_0;
	struct s_Polynomial	*target_1;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	target_0 = numeric_pow(v0, r);
	TEST_ASSERT_TRUE(Polynomial_size(target_0) == 1);
	TEST_ASSERT_TRUE(numeric_equal(Polynomial_at(target_0, 0), coeff_0));

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 2, r1);
	target_1 = numeric_pow(v1, r);
	TEST_ASSERT_TRUE(Polynomial_size(target_1) == 7);
	TEST_ASSERT_TRUE(numeric_equal(Polynomial_at(target_1, 6), coeff_1));

	delete(r);
	delete(coeff_0);
	delete(coeff_1);
	delete(v0);
	delete(v1);
	delete(target_0);
	delete(target_1);
}

void test_equal_true(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 2);
	struct s_Rational	*r3 = Rational_from_long(3, 4);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v1;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 0, r2);
	Polynomial_update(v1, 1, r3);

	TEST_ASSERT_TRUE(equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_equal_false(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long((long)-1, 2);
	struct s_Rational	*r3 = Rational_from_long((long)-3, 4);
	struct s_Polynomial		*v0;
	struct s_Polynomial		*v1;

	v0 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v0, 0, r0);
	Polynomial_update(v0, 1, r1);

	v1 = new(Polynomial, POLYNOMIAL);
	Polynomial_update(v1, 0, r2);
	Polynomial_update(v1, 1, r3);

	TEST_ASSERT_FALSE(equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_promote_rational(void) {
	struct s_Complex	*val = Rational_from_long(1, 2);
	void				*a;
	struct s_Rational	*b;

	a = new(Polynomial, POLYNOMIAL);
	Polynomial_update(a, 0, val);

	b = numeric_promote(a, RATIONAL);
	TEST_ASSERT_NULL(b);
	delete(a);
}

void test_promote_complex(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	void				*a;
	struct s_Complex	*b;

	a = new(Polynomial, POLYNOMIAL);
	Polynomial_update(a, 0, val);

	b = numeric_promote(a, COMPLEX);
	TEST_ASSERT_NULL(b);
	delete(a);
}

void test_promote_vector(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	void				*a;
	struct s_Complex	*b;

	a = new(Polynomial, POLYNOMIAL);
	Polynomial_update(a, 0, val);

	b = numeric_promote(a, POLYNOMIAL);
	TEST_ASSERT_TRUE(equal(b, a));
	delete(a);
	delete(b);
}

void test_promote_matrix(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	void				*a;
	void				*b;

	a = new(Polynomial, POLYNOMIAL);
	Polynomial_update(a, 0, val);

	b = numeric_promote(a, MATRIX);
	TEST_ASSERT_NULL(b);
	delete(a);
	delete(b);
}

void test_iszero(void) {
	struct s_Complex	*val1 = Rational_from_long(1, 2);
	struct s_Complex	*val2 = Rational_from_long(0, 1);
	void				*a;
	void				*b;
	void				*c;
	void				*d;

	a = new(Polynomial, POLYNOMIAL);
	Polynomial_update(a, 0, val1);
	TEST_ASSERT_FALSE(numeric_iszero(a));

	b = new(Polynomial, POLYNOMIAL);
	Polynomial_update(b, 0, val2);
	TEST_ASSERT_TRUE(numeric_iszero(b));

	c = new(Polynomial, POLYNOMIAL);
	Polynomial_update(c, 0, copy(val1));
	Polynomial_update(c, 1, copy(val2));
	TEST_ASSERT_FALSE(numeric_iszero(c));

	d = new(Polynomial, POLYNOMIAL);
	Polynomial_update(d, 0, copy(val2));
	Polynomial_update(d, 1, copy(val2));
	TEST_ASSERT_TRUE(numeric_iszero(d));

	delete(a);
	delete(b);
	delete(c);
	delete(d);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_ctor);
	RUN_TEST(test_str);
	RUN_TEST(test_add_scalar);
	RUN_TEST(test_add_polynomial);
	RUN_TEST(test_sub_scalar);
	RUN_TEST(test_sub_polynomial);
	RUN_TEST(test_mul_scalar);
	RUN_TEST(test_mul_polynomial);
	RUN_TEST(test_div_scalar);
	RUN_TEST(test_div_polynomial);
	RUN_TEST(test_mod_scalar);
	RUN_TEST(test_mod_polynomial);
	RUN_TEST(test_neg);
	RUN_TEST(test_pow);
	RUN_TEST(test_equal_true);
	RUN_TEST(test_equal_false);
	RUN_TEST(test_promote_rational);
	RUN_TEST(test_promote_complex);
	RUN_TEST(test_promote_vector);
	RUN_TEST(test_promote_matrix);
	RUN_TEST(test_iszero);
	return UNITY_END();
}
