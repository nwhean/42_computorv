#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"

/* container */
#include "Vec.h"

/* lexer */
#include "Complex.h"
#include "Numeric.h"
#include "Rational.h"
#include "Vector.h"
#include "Matrix.h"

void setUp(void) {
	initComplex();
	initRational();
	initVector();
	initMatrix();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(5, 6);
	struct s_Vector		*v;

	v = new(Vector, VECTOR, 3);
	Vector_update(v, 0, r0);
	Vector_update(v, 1, r1);
	Vector_update(v, 2, r2);
	TEST_ASSERT_TRUE(numeric_equal(r0, Vector_at(v, 0)));
	TEST_ASSERT_TRUE(numeric_equal(r1, Vector_at(v, 1)));
	TEST_ASSERT_TRUE(numeric_equal(r2, Vector_at(v, 2)));
	delete(v);
}

void test_str(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(5, 6);
	struct s_Vector		*v;
	char				*s;
	char				*target;

	v = new(Vector, VECTOR, 3);
	Vector_update(v, 0, r0);
	Vector_update(v, 1, r1);
	Vector_update(v, 2, r2);
	s = str(v);
	target = "[0.500000, 0.750000, 0.833333]";
	TEST_ASSERT_EQUAL_STRING_LEN(target, s, 30);
	free((void *)s);
	delete(v);
}

void test_add_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r = Rational_from_long(1, 2);
	struct s_Vector		*v;

	v = new(Vector, VECTOR, 2);
	Vector_update(v, 0, r0);
	Vector_update(v, 1, r1);
	TEST_ASSERT_NULL(numeric_add(v, r));
	delete(r);
	delete(v);
}

void test_add_vector(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 1);
	struct s_Rational	*r3 = Rational_from_long(3, 2);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

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
	struct s_Vector		*v;

	v = new(Vector, VECTOR, 2);
	Vector_update(v, 0, r0);
	Vector_update(v, 1, r1);
	TEST_ASSERT_NULL(numeric_sub(v, r));
	delete(r);
	delete(v);
}

void test_sub_vector(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(0, 1);
	struct s_Rational	*r3 = Rational_from_long(0, 1);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = numeric_sub(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_mul_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Vector		*v0;

	struct s_Rational	*r2 = Rational_from_long(7, 2);
	struct s_Rational	*r3 = Rational_from_long(21, 4);
	struct s_Vector		*v1;

	struct s_Rational	*r = Rational_from_long(7, 1);
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = numeric_mul(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_mul_vector(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 4);
	struct s_Rational	*r3 = Rational_from_long(9, 16);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = numeric_mul(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_div_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 14);
	struct s_Rational	*r3 = Rational_from_long(3, 28);
	struct s_Rational	*r = Rational_from_long(7, 1);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = numeric_div(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_div_vector(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 1);
	struct s_Rational	*r3 = Rational_from_long(1, 1);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = numeric_div(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_mod_scalar(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(0, 1);
	struct s_Rational	*r3 = Rational_from_long(1, 4);
	struct s_Rational	*r = Rational_from_long(1, 2);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = numeric_mod(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_mod_vector(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Vector		*v0;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	TEST_ASSERT_NULL(numeric_mod(v0, v0));
	delete(v0);
}

void test_neg(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long((long)-1, 2);
	struct s_Rational	*r3 = Rational_from_long((long)-3, 4);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = numeric_neg(v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_pow(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Vector		*v0;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	TEST_ASSERT_NULL(numeric_pow(v0, v0));
	delete(v0);
}

void test_equal_true(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(1, 2);
	struct s_Rational	*r3 = Rational_from_long(3, 4);
	struct s_Vector		*v0;
	struct s_Vector		*v1;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	TEST_ASSERT_TRUE(equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_equal_false(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long((long)-1, 2);
	struct s_Rational	*r3 = Rational_from_long((long)-3, 4);
	struct s_Vector		*v0;
	struct s_Vector		*v1;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	TEST_ASSERT_FALSE(equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_promote_rational(void) {
	struct s_Complex	*val = Rational_from_long(1, 2);
	void				*a;
	struct s_Rational	*b;

	a = new(Vector, VECTOR, 1);
	Vector_update(a, 0, val);

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

	a = new(Vector, VECTOR, 1);
	Vector_update(a, 0, val);

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

	a = new(Vector, VECTOR, 1);
	Vector_update(a, 0, val);

	b = numeric_promote(a, VECTOR);
	TEST_ASSERT_TRUE(equal(b, a));
	delete(a);
	delete(b);
}

void test_promote_matrix(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	void				*a;

	struct s_Complex	*val_t = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	void				*vec_t = new(Vec);
	void				*vec_m = new(Vec);
	void				*target;

	struct s_Matrix		*b;

	a = new(Vector, VECTOR, 1);
	Vector_update(a, 0, val);

	Vec_push_back(vec_t, val_t);
	Vec_push_back(vec_m, vec_t);
	target = new(Matrix, MATRIX, vec_m);

	b = numeric_promote(a, MATRIX);
	TEST_ASSERT_TRUE(equal(b, target));
	delete(a);
	delete(b);
	delete(target);
}

void test_conjugate_rational(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Vector		*v0;
	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v = Vector_conjugate(v0);
	TEST_ASSERT_TRUE(equal(v, v0));
	delete(v0);
	delete(v);
}

void test_conjugate_complex(void) {
	struct s_Complex	*r0 = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long(3, 4));
	struct s_Complex	*r1 = new(Complex, COMPLEX,
								Rational_from_long(5, 6),
								Rational_from_long(7, 8));
	struct s_Vector		*v0;

	struct s_Complex	*r2 = new(Complex, COMPLEX,
								Rational_from_long(1, 2),
								Rational_from_long((long)-3, 4));
	struct s_Complex	*r3 = new(Complex, COMPLEX,
								Rational_from_long(5, 6),
								Rational_from_long((long)-7, 8));
	struct s_Vector		*v1;

	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r0);
	Vector_update(v0, 1, r1);

	v1 = new(Vector, VECTOR, 2);
	Vector_update(v1, 0, r2);
	Vector_update(v1, 1, r3);

	v = Vector_conjugate(v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_cross(void) {
	struct s_Rational	*r00 = Rational_from_long((long)-1, 1);
	struct s_Rational	*r01 = Rational_from_long(2, 1);
	struct s_Rational	*r02 = Rational_from_long(5, 1);
	struct s_Vector		*v0;

	struct s_Rational	*r10 = Rational_from_long(4, 1);
	struct s_Rational	*r11 = Rational_from_long(0, 1);
	struct s_Rational	*r12 = Rational_from_long((long)-3, 1);
	struct s_Vector		*v1;

	struct s_Rational	*r20 = Rational_from_long((long)-6, 1);
	struct s_Rational	*r21 = Rational_from_long(17, 1);
	struct s_Rational	*r22 = Rational_from_long((long)-8, 1);
	struct s_Vector		*v2;

	struct s_Vector		*v;

	v0 = new(Vector, VECTOR, 3);
	Vector_update(v0, 0, r00);
	Vector_update(v0, 1, r01);
	Vector_update(v0, 2, r02);

	v1 = new(Vector, VECTOR, 3);
	Vector_update(v1, 0, r10);
	Vector_update(v1, 1, r11);
	Vector_update(v1, 2, r12);

	v2 = new(Vector, VECTOR, 3);
	Vector_update(v2, 0, r20);
	Vector_update(v2, 1, r21);
	Vector_update(v2, 2, r22);

	v = Vector_cross(v0, v1);
	TEST_ASSERT_TRUE(equal(v, v2));
	delete(v0);
	delete(v1);
	delete(v2);
	delete(v);
}

void test_dot(void) {
	struct s_Rational	*r00 = Rational_from_long(10, 1);
	struct s_Rational	*r01 = Rational_from_long((long)-4, 1);
	struct s_Rational	*r02 = Rational_from_long(7, 1);
	struct s_Vector		*v0;

	struct s_Rational	*r10 = Rational_from_long((long)-2, 1);
	struct s_Rational	*r11 = Rational_from_long(1, 1);
	struct s_Rational	*r12 = Rational_from_long(6, 1);
	struct s_Vector		*v1;

	struct s_Rational	*target = Rational_from_long(18, 1);
	struct s_Rational	*r;

	v0 = new(Vector, VECTOR, 3);
	Vector_update(v0, 0, r00);
	Vector_update(v0, 1, r01);
	Vector_update(v0, 2, r02);

	v1 = new(Vector, VECTOR, 3);
	Vector_update(v1, 0, r10);
	Vector_update(v1, 1, r11);
	Vector_update(v1, 2, r12);

	r = Vector_dot(v0, v1);
	TEST_ASSERT_TRUE(equal(r, target));
	delete(v0);
	delete(v1);
	delete(r);
	delete(target);
}

void test_sum(void) {
	struct s_Rational	*r00 = Rational_from_long(10, 1);
	struct s_Rational	*r01 = Rational_from_long((long)-4, 1);
	struct s_Rational	*r02 = Rational_from_long(7, 1);
	struct s_Vector		*v0;

	struct s_Rational	*target = Rational_from_long(13, 1);
	struct s_Rational	*r;

	v0 = new(Vector, VECTOR, 3);
	Vector_update(v0, 0, r00);
	Vector_update(v0, 1, r01);
	Vector_update(v0, 2, r02);

	r = Vector_sum(v0);
	TEST_ASSERT_TRUE(equal(r, target));
	delete(v0);
	delete(r);
	delete(target);
}

void test_magnitude(void) {
	struct s_Rational	*r00 = Rational_from_long((long)-3, 1);
	struct s_Rational	*r01 = Rational_from_long((long)4, 1);
	struct s_Vector		*v0;
	struct s_Rational	*r;

	v0 = new(Vector, VECTOR, 2);
	Vector_update(v0, 0, r00);
	Vector_update(v0, 1, r01);

	r = Vector_magnitude(v0);
	TEST_ASSERT_FLOAT_WITHIN(0.01, 1, Rational_to_double(r) / 5);
	delete(v0);
	delete(r);
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
	RUN_TEST(test_promote_rational);
	RUN_TEST(test_promote_complex);
	RUN_TEST(test_promote_vector);
	RUN_TEST(test_promote_matrix);
	RUN_TEST(test_conjugate_rational);
	RUN_TEST(test_conjugate_complex);
	RUN_TEST(test_cross);
	RUN_TEST(test_dot);
	RUN_TEST(test_magnitude);
	RUN_TEST(test_sum);
	return UNITY_END();
}
