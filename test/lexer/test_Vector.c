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
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	void				*vec = new(Vec);
	struct s_Vector		*v;

	Vec_push_back(vec, r0);
	Vec_push_back(vec, r1);
	Vec_push_back(vec, r2);
	v = new(Vector, VECTOR, vec);
	TEST_ASSERT_TRUE(numeric_equal(r0, Vector_at(v, 0)));
	TEST_ASSERT_TRUE(numeric_equal(r1, Vector_at(v, 1)));
	TEST_ASSERT_TRUE(numeric_equal(r2, Vector_at(v, 2)));
	delete(v);
}

void test_str(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	void				*vec = new(Vec);
	struct s_Vector		*v;
	char				*s;
	char				*target;

	Vec_push_back(vec, r0);
	Vec_push_back(vec, r1);
	Vec_push_back(vec, r2);
	v = new(Vector, VECTOR, vec);
	s = str(v);
	target = "[0.500000, 0.750000, 0.833333]";
	TEST_ASSERT_EQUAL_STRING_LEN(target, s, 30);
	free((void *)s);
	delete(v);
}

void test_add_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	void				*vec = new(Vec);
	struct s_Vector		*v;

	Vec_push_back(vec, r0);
	Vec_push_back(vec, r1);
	v = new(Vector, VECTOR, vec);
	TEST_ASSERT_NULL(numeric_add(v, r));
	delete(r);
	delete(v);
}

void test_add_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 2);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	v = numeric_add(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_sub_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	void				*vec = new(Vec);
	struct s_Vector		*v;

	Vec_push_back(vec, r0);
	Vec_push_back(vec, r1);
	v = new(Vector, VECTOR, vec);
	TEST_ASSERT_NULL(numeric_sub(v, r));
	delete(r);
	delete(v);
}

void test_sub_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 0, 1);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	v = numeric_sub(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_mul_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	struct s_Rational	*r2 = new(Rational, RATIONAL, 7, 2);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 21, 4);
	void				*vec1 = new(Vec);
	struct s_Vector		*v1;

	struct s_Rational	*r = new(Rational, RATIONAL, 7, 1);
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);

	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);

	v = numeric_mul(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_mul_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 4);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 9, 16);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	v = numeric_mul(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_div_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 14);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 28);
	struct s_Rational	*r = new(Rational, RATIONAL, 7, 1);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	v = numeric_div(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_div_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 1, 1);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	v = numeric_div(v0, v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_mod_scalar(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 1, 4);
	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	v = numeric_mod(v0, r);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
	delete(r);
}

void test_mod_vector(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	TEST_ASSERT_NULL(numeric_mod(v0, v0));
	delete(v0);
}

void test_neg(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, (long)-1, 2);
	struct s_Rational	*r3 = new(Rational, RATIONAL, (long)-3, 4);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	v = numeric_neg(v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_pow(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	TEST_ASSERT_NULL(numeric_pow(v0, v0));
	delete(v0);
}

void test_equal_true(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 3, 4);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	TEST_ASSERT_TRUE(equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_equal_false(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, (long)-1, 2);
	struct s_Rational	*r3 = new(Rational, RATIONAL, (long)-3, 4);
	void				*vec1 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v1;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);
	TEST_ASSERT_FALSE(equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_promote_rational(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, 3, 4));
	void				*vec = new(Vec);
	void				*a;
	struct s_Rational	*b;

	Vec_push_back(vec, val);
	a = new(Vector, VECTOR, vec);
	b = numeric_promote(a, RATIONAL);
	TEST_ASSERT_NULL(b);
	delete(a);
}

void test_promote_complex(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, 3, 4));
	void				*vec = new(Vec);
	void				*a;
	struct s_Complex	*b;

	Vec_push_back(vec, val);
	a = new(Vector, VECTOR, vec);
	b = numeric_promote(a, COMPLEX);
	TEST_ASSERT_NULL(b);
	delete(a);
}

void test_promote_vector(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, 3, 4));
	void				*vec = new(Vec);
	void				*a;

	struct s_Complex	*val_t = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, 3, 4));
	void				*vec_t = new(Vec);
	void				*target;

	struct s_Vector		*b;

	Vec_push_back(vec, val);
	a = new(Vector, VECTOR, vec);

	Vec_push_back(vec_t, val_t);
	target = new(Vector, VECTOR, vec_t);

	b = numeric_promote(a, VECTOR);
	TEST_ASSERT_TRUE(equal(b, target));
	delete(a);
	delete(b);
	delete(target);
}

void test_promote_matrix(void) {
	struct s_Complex	*val = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, 3, 4));
	void				*vec = new(Vec);
	void				*a;

	struct s_Complex	*val_t = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, 3, 4));
	void				*vec_t = new(Vec);
	void				*vec_m = new(Vec);
	void				*target;

	struct s_Matrix		*b;

	Vec_push_back(vec, val);
	a = new(Vector, VECTOR, vec);

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
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;
	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);
	v = Vector_conjugate(v0);
	TEST_ASSERT_TRUE(equal(v, v0));
	delete(v0);
	delete(v);
}

void test_conjugate_complex(void) {
	struct s_Complex	*r0 = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, 3, 4));
	struct s_Complex	*r1 = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 5, 6),
								new(Rational, RATIONAL, 7, 8));
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	struct s_Complex	*r2 = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 1, 2),
								new(Rational, RATIONAL, (long)-3, 4));
	struct s_Complex	*r3 = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 5, 6),
								new(Rational, RATIONAL, (long)-7, 8));
	void				*vec1 = new(Vec);
	struct s_Vector		*v1;

	struct s_Vector		*v;

	Vec_push_back(vec0, r0);
	Vec_push_back(vec0, r1);
	v0 = new(Vector, VECTOR, vec0);

	Vec_push_back(vec1, r2);
	Vec_push_back(vec1, r3);
	v1 = new(Vector, VECTOR, vec1);

	v = Vector_conjugate(v0);
	TEST_ASSERT_TRUE(equal(v, v1));
	delete(v0);
	delete(v1);
	delete(v);
}

void test_cross(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, (long)-1, 1);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 2, 1);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 1);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, 4, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r12 = new(Rational, RATIONAL, (long)-3, 1);
	void				*vec1 = new(Vec);
	struct s_Vector		*v1;

	struct s_Rational	*r20 = new(Rational, RATIONAL, (long)-6, 1);
	struct s_Rational	*r21 = new(Rational, RATIONAL, 17, 1);
	struct s_Rational	*r22 = new(Rational, RATIONAL, (long)-8, 1);
	void				*vec2 = new(Vec);
	struct s_Vector		*v2;

	struct s_Vector		*v;

	Vec_push_back(vec0, r00);
	Vec_push_back(vec0, r01);
	Vec_push_back(vec0, r02);
	v0 = new(Vector, VECTOR, vec0);

	Vec_push_back(vec1, r10);
	Vec_push_back(vec1, r11);
	Vec_push_back(vec1, r12);
	v1 = new(Vector, VECTOR, vec1);

	Vec_push_back(vec2, r20);
	Vec_push_back(vec2, r21);
	Vec_push_back(vec2, r22);
	v2 = new(Vector, VECTOR, vec2);

	v = Vector_cross(v0, v1);
	TEST_ASSERT_TRUE(equal(v, v2));
	delete(v0);
	delete(v1);
	delete(v2);
	delete(v);
}

void test_dot(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 10, 1);
	struct s_Rational	*r01 = new(Rational, RATIONAL, (long)-4, 1);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 7, 1);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, (long)-2, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 6, 1);
	void				*vec1 = new(Vec);
	struct s_Vector		*v1;

	struct s_Rational	*target = new(Rational, RATIONAL, 18, 1);
	struct s_Rational	*r;

	Vec_push_back(vec0, r00);
	Vec_push_back(vec0, r01);
	Vec_push_back(vec0, r02);
	v0 = new(Vector, VECTOR, vec0);

	Vec_push_back(vec1, r10);
	Vec_push_back(vec1, r11);
	Vec_push_back(vec1, r12);
	v1 = new(Vector, VECTOR, vec1);

	r = Vector_dot(v0, v1);
	TEST_ASSERT_TRUE(equal(r, target));
	delete(v0);
	delete(v1);
	delete(r);
	delete(target);
}

void test_sum(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 10, 1);
	struct s_Rational	*r01 = new(Rational, RATIONAL, (long)-4, 1);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 7, 1);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	struct s_Rational	*target = new(Rational, RATIONAL, 13, 1);
	struct s_Rational	*r;

	Vec_push_back(vec0, r00);
	Vec_push_back(vec0, r01);
	Vec_push_back(vec0, r02);
	v0 = new(Vector, VECTOR, vec0);

	r = Vector_sum(v0);
	TEST_ASSERT_TRUE(equal(r, target));
	delete(v0);
	delete(r);
	delete(target);
}

void test_magnitude(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, (long)-3, 1);
	struct s_Rational	*r01 = new(Rational, RATIONAL, (long)4, 1);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	struct s_Rational	*r;

	Vec_push_back(vec0, r00);
	Vec_push_back(vec0, r01);
	v0 = new(Vector, VECTOR, vec0);

	r = Vector_magnitude(v0);
	TEST_ASSERT_FLOAT_WITHIN(0.01, 1, Rational_to_double(r) / 5);
	delete(v0);
	delete(r);
}

void test_sum_product(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 10, 1);
	struct s_Rational	*r01 = new(Rational, RATIONAL, (long)-4, 1);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 7, 1);
	void				*vec0 = new(Vec);
	struct s_Vector		*v0;

	struct s_Complex	*r10 = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 0, 1),
								new(Rational, RATIONAL, (long)-2, 1));
	struct s_Complex	*r11 = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 0, 1),
								new(Rational, RATIONAL, 1, 1));
	struct s_Complex	*r12 = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 0, 1),
								new(Rational, RATIONAL, 6, 1));
	void				*vec1 = new(Vec);
	struct s_Vector		*v1;

	struct s_Complex	*target = new(Complex, COMPLEX,
								new(Rational, RATIONAL, 0, 1),
								new(Rational, RATIONAL, 18, 1));
	struct s_Complex	*r;

	Vec_push_back(vec0, r00);
	Vec_push_back(vec0, r01);
	Vec_push_back(vec0, r02);
	v0 = new(Vector, VECTOR, vec0);

	Vec_push_back(vec1, r10);
	Vec_push_back(vec1, r11);
	Vec_push_back(vec1, r12);
	v1 = new(Vector, VECTOR, vec1);

	r = Vector_sum_product(v0, v1);
	TEST_ASSERT_TRUE(equal(r, target));
	delete(v0);
	delete(v1);
	delete(r);
	delete(target);
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
	RUN_TEST(test_sum_product);
	return UNITY_END();
}
