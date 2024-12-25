#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"

/* container */
#include "Vec.h"

/* lexer */
#include "Numeric.h"
#include "Rational.h"
#include "Matrix.h"
#include "Vector.h"

void setUp(void) {
	initRational();
	initMatrix();
	initVector();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(5, 6);
	struct s_Rational	*r3 = Rational_from_long(7, 8);
	void				*v0 = new(Vec);
	void				*v1 = new(Vec);
	void				*v = new(Vec);
	struct s_Matrix		*m;

	Vec_push_back(v0, r0);
	Vec_push_back(v0, r1);
	Vec_push_back(v1, r2);
	Vec_push_back(v1, r3);
	Vec_push_back(v, v0);
	Vec_push_back(v, v1);
	m = new(Matrix, MATRIX, v);
	TEST_ASSERT_TRUE(numeric_equal(r0, Matrix_at(m, 0, 0)));
	TEST_ASSERT_TRUE(numeric_equal(r1, Matrix_at(m, 0, 1)));
	TEST_ASSERT_TRUE(numeric_equal(r2, Matrix_at(m, 1, 0)));
	TEST_ASSERT_TRUE(numeric_equal(r3, Matrix_at(m, 1, 1)));
	delete(m);
}

void test_str(void) {
	struct s_Rational	*r0 = Rational_from_long(1, 2);
	struct s_Rational	*r1 = Rational_from_long(3, 4);
	struct s_Rational	*r2 = Rational_from_long(5, 6);
	struct s_Rational	*r3 = Rational_from_long(7, 8);
	void				*v0 = new(Vec);
	void				*v1 = new(Vec);
	void				*v = new(Vec);
	struct s_Matrix		*m;
	char				*s;
	char				*target;

	Vec_push_back(v0, r0);
	Vec_push_back(v0, r1);
	Vec_push_back(v1, r2);
	Vec_push_back(v1, r3);
	Vec_push_back(v, v0);
	Vec_push_back(v, v1);
	m = new(Matrix, MATRIX, v);
	s = str(m);
	target = "[0.500000, 0.750000]\n[0.833333, 0.875000]";
	TEST_ASSERT_EQUAL_STRING(target, s);
	free((void *)s);
	delete(m);
}

void test_add_scalar(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = Rational_from_long(1, 2);

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	TEST_ASSERT_NULL(numeric_add(m0, r));
	delete(m0);
	delete(r);
}

void test_add_matrix(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(1, 1);
	struct s_Rational	*r11 = Rational_from_long(3, 2);
	struct s_Rational	*r12 = Rational_from_long(5, 3);
	struct s_Rational	*r13 = Rational_from_long(7, 4);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_add(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_sub_scalar(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = Rational_from_long(1, 2);

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	TEST_ASSERT_NULL(numeric_sub(m0, r));
	delete(m0);
	delete(r);
}

void test_sub_matrix(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(0, 1);
	struct s_Rational	*r11 = Rational_from_long(0, 1);
	struct s_Rational	*r12 = Rational_from_long(0, 1);
	struct s_Rational	*r13 = Rational_from_long(0, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_sub(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_mul_scalar(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = Rational_from_long(1, 2);

	struct s_Rational	*r10 = Rational_from_long(1, 4);
	struct s_Rational	*r11 = Rational_from_long(3, 8);
	struct s_Rational	*r12 = Rational_from_long(5, 12);
	struct s_Rational	*r13 = Rational_from_long(7, 16);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_mul(m0, r);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(r);
	delete(m);
}

void test_mul_matrix(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(1, 4);
	struct s_Rational	*r11 = Rational_from_long(9, 16);
	struct s_Rational	*r12 = Rational_from_long(25, 36);
	struct s_Rational	*r13 = Rational_from_long(49, 64);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_mul(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_div_scalar(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = Rational_from_long(1, 2);

	struct s_Rational	*r10 = Rational_from_long(1, 1);
	struct s_Rational	*r11 = Rational_from_long(3, 2);
	struct s_Rational	*r12 = Rational_from_long(5, 3);
	struct s_Rational	*r13 = Rational_from_long(7, 4);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_div(m0, r);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(r);
	delete(m);
}

void test_div_matrix(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(1, 1);
	struct s_Rational	*r11 = Rational_from_long(1, 1);
	struct s_Rational	*r12 = Rational_from_long(1, 1);
	struct s_Rational	*r13 = Rational_from_long(1, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_div(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_mod_scalar(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = Rational_from_long(1, 2);

	struct s_Rational	*r10 = Rational_from_long(0, 1);
	struct s_Rational	*r11 = Rational_from_long(1, 4);
	struct s_Rational	*r12 = Rational_from_long(1, 3);
	struct s_Rational	*r13 = Rational_from_long(3, 8);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_mod(m0, r);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(r);
	delete(m);
}

void test_mod_matrix(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	m = numeric_mod(m0, m0);
	TEST_ASSERT_NULL(m);
	delete(m0);
}

void test_neg(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long((long)-1, 2);
	struct s_Rational	*r11 = Rational_from_long((long)-3, 4);
	struct s_Rational	*r12 = Rational_from_long((long)-5, 6);
	struct s_Rational	*r13 = Rational_from_long((long)-7, 8);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_neg(m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_pow(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 1);
	struct s_Rational	*r01 = Rational_from_long(2, 1);
	struct s_Rational	*r02 = Rational_from_long(3, 1);
	struct s_Rational	*r03 = Rational_from_long(4, 1);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(37, 1);
	struct s_Rational	*r11 = Rational_from_long(54, 1);
	struct s_Rational	*r12 = Rational_from_long(81, 1);
	struct s_Rational	*r13 = Rational_from_long(118, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Rational	*r = Rational_from_long(3, 1);
	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_pow(m0, r);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
	delete(r);
}

void test_equal_true(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(1, 2);
	struct s_Rational	*r11 = Rational_from_long(3, 4);
	struct s_Rational	*r12 = Rational_from_long(5, 6);
	struct s_Rational	*r13 = Rational_from_long(7, 8);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);
	TEST_ASSERT_TRUE(numeric_equal(m0, m1));
	delete(m0);
	delete(m1);
}

void test_equal_false(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(1, 2);
	struct s_Rational	*r11 = Rational_from_long(3, 4);
	struct s_Rational	*r12 = Rational_from_long(5, 6);
	struct s_Rational	*r13 = Rational_from_long(7, 9);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);
	TEST_ASSERT_FALSE(numeric_equal(m0, m1));
	delete(m0);
	delete(m1);
}

void test_promote_rational(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;
	struct s_Matrix		*m1;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	m1 = numeric_promote(m0, RATIONAL);
	TEST_ASSERT_NULL(m1);
	delete(m0);
	delete(m1);
}

void test_promote_complex(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;
	struct s_Matrix		*m1;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	m1 = numeric_promote(m0, COMPLEX);
	TEST_ASSERT_NULL(m1);
	delete(m0);
	delete(m1);
}

void test_promote_vector(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;
	struct s_Matrix		*m1;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	m1 = numeric_promote(m0, VECTOR);
	TEST_ASSERT_NULL(m1);
	delete(m0);
	delete(m1);
}

void test_promote_matrix(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 2);
	struct s_Rational	*r01 = Rational_from_long(3, 4);
	struct s_Rational	*r02 = Rational_from_long(5, 6);
	struct s_Rational	*r03 = Rational_from_long(7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(1, 2);
	struct s_Rational	*r11 = Rational_from_long(3, 4);
	struct s_Rational	*r12 = Rational_from_long(5, 6);
	struct s_Rational	*r13 = Rational_from_long(7, 8);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*target;

	struct s_Matrix		*m1;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	target = new(Matrix, MATRIX, v1);

	m1 = numeric_promote(m0, MATRIX);
	TEST_ASSERT_TRUE(numeric_equal(m1, target));
	delete(m0);
	delete(m1);
	delete(target);
}

void test_eye(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 1);
	struct s_Rational	*r01 = Rational_from_long(0, 1);
	struct s_Rational	*r02 = Rational_from_long(0, 1);
	struct s_Rational	*r03 = Rational_from_long(1, 1);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	m0 = new(Matrix, MATRIX, v0);

	m = Matrix_eye(2);
	TEST_ASSERT_TRUE(numeric_equal(m0, m));
	delete(m0);
	delete(m);
}

void test_mmul(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 1);
	struct s_Rational	*r01 = Rational_from_long(2, 1);
	void				*v00 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(3, 1);
	struct s_Rational	*r11 = Rational_from_long(4, 1);
	struct s_Rational	*r12 = Rational_from_long(5, 1);
	struct s_Rational	*r13 = Rational_from_long(6, 1);
	struct s_Rational	*r14 = Rational_from_long(7, 1);
	struct s_Rational	*r15 = Rational_from_long(8, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Rational	*r20 = Rational_from_long(15, 1);
	struct s_Rational	*r21 = Rational_from_long(18, 1);
	struct s_Rational	*r22 = Rational_from_long(21, 1);
	void				*v20 = new(Vec);
	void				*v2 = new(Vec);
	struct s_Matrix		*m2;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v0, v00);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v10, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v11, r14);
	Vec_push_back(v11, r15);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	Vec_push_back(v20, r20);
	Vec_push_back(v20, r21);
	Vec_push_back(v20, r22);
	Vec_push_back(v2, v20);
	m2 = new(Matrix, MATRIX, v2);

	m = Matrix_mmult(m0, m1);
	TEST_ASSERT_TRUE(numeric_equal(m, m2));
	delete(m0);
	delete(m1);
	delete(m2);
	delete(m);
}

void test_transpose(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 1);
	struct s_Rational	*r01 = Rational_from_long(2, 1);
	void				*v00 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(1, 1);
	struct s_Rational	*r11 = Rational_from_long(2, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v0, v00);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v11, r11);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	m1 = new(Matrix, MATRIX, v1);

	m = Matrix_transpose(m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_solve(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 1);
	struct s_Rational	*r01 = Rational_from_long(2, 1);
	struct s_Rational	*r02 = Rational_from_long(3, 1);
	struct s_Rational	*r03 = Rational_from_long(4, 1);
	struct s_Rational	*r04 = Rational_from_long(5, 1);
	struct s_Rational	*r05 = Rational_from_long(6, 1);
	struct s_Rational	*r06 = Rational_from_long(7, 1);
	struct s_Rational	*r07 = Rational_from_long(8, 1);
	struct s_Rational	*r08 = Rational_from_long(10, 1);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v02 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long(2, 1);
	struct s_Rational	*r11 = Rational_from_long(4, 1);
	struct s_Rational	*r12 = Rational_from_long(6, 1);
	void				*v1 = new(Vec);
	struct s_Vector		*b;

	struct s_Rational	*r20 = Rational_from_long((long)-2, 3);
	struct s_Rational	*r21 = Rational_from_long(4, 3);
	struct s_Rational	*r22 = Rational_from_long(0, 1);
	void				*v2 = new(Vec);
	struct s_Vector		*target;

	struct s_Vector		*x;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v00, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v01, r04);
	Vec_push_back(v01, r05);
	Vec_push_back(v02, r06);
	Vec_push_back(v02, r07);
	Vec_push_back(v02, r08);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	Vec_push_back(v0, v02);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v1, r10);
	Vec_push_back(v1, r11);
	Vec_push_back(v1, r12);
	b = new(Vector, VECTOR, v1);

	Vec_push_back(v2, r20);
	Vec_push_back(v2, r21);
	Vec_push_back(v2, r22);
	target = new(Vector, VECTOR, v2);

	x = Matrix_solve(m0, b);
	TEST_ASSERT_TRUE(numeric_equal(x, target));
	delete(m0);
	delete(b);
	delete(target);
	delete(x);
}

void test_inverse(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 1);
	struct s_Rational	*r01 = Rational_from_long(2, 1);
	struct s_Rational	*r02 = Rational_from_long(3, 1);
	struct s_Rational	*r03 = Rational_from_long(4, 1);
	struct s_Rational	*r04 = Rational_from_long(5, 1);
	struct s_Rational	*r05 = Rational_from_long(6, 1);
	struct s_Rational	*r06 = Rational_from_long(7, 1);
	struct s_Rational	*r07 = Rational_from_long(8, 1);
	struct s_Rational	*r08 = Rational_from_long(10, 1);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v02 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = Rational_from_long((long)-2, 3);
	struct s_Rational	*r11 = Rational_from_long((long)-4, 3);
	struct s_Rational	*r12 = Rational_from_long(1, 1);
	struct s_Rational	*r13 = Rational_from_long((long)-2, 3);
	struct s_Rational	*r14 = Rational_from_long(11, 3);
	struct s_Rational	*r15 = Rational_from_long((long)-2, 1);
	struct s_Rational	*r16 = Rational_from_long(1, 1);
	struct s_Rational	*r17 = Rational_from_long((long)-2, 1);
	struct s_Rational	*r18 = Rational_from_long(1, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v12 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v00, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v01, r04);
	Vec_push_back(v01, r05);
	Vec_push_back(v02, r06);
	Vec_push_back(v02, r07);
	Vec_push_back(v02, r08);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	Vec_push_back(v0, v02);
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v10, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v11, r14);
	Vec_push_back(v11, r15);
	Vec_push_back(v12, r16);
	Vec_push_back(v12, r17);
	Vec_push_back(v12, r18);
	Vec_push_back(v1, v10);
	Vec_push_back(v1, v11);
	Vec_push_back(v1, v12);
	m1 = new(Matrix, MATRIX, v1);

	m = Matrix_invert(m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_determinant(void) {
	struct s_Rational	*r00 = Rational_from_long(1, 1);
	struct s_Rational	*r01 = Rational_from_long(2, 1);
	struct s_Rational	*r02 = Rational_from_long(3, 1);
	struct s_Rational	*r03 = Rational_from_long(4, 1);
	struct s_Rational	*r04 = Rational_from_long(5, 1);
	struct s_Rational	*r05 = Rational_from_long(6, 1);
	struct s_Rational	*r06 = Rational_from_long(7, 1);
	struct s_Rational	*r07 = Rational_from_long(8, 1);
	struct s_Rational	*r08 = Rational_from_long(10, 1);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v02 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*target = Rational_from_long((long)-3, 1);
	struct s_Rational	*r;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v00, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v01, r04);
	Vec_push_back(v01, r05);
	Vec_push_back(v02, r06);
	Vec_push_back(v02, r07);
	Vec_push_back(v02, r08);
	Vec_push_back(v0, v00);
	Vec_push_back(v0, v01);
	Vec_push_back(v0, v02);
	m0 = new(Matrix, MATRIX, v0);

	r = Matrix_determinant(m0);
	TEST_ASSERT_TRUE(numeric_equal(r, target));
	delete(m0);
	delete(r);
	delete(target);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_ctor);
	RUN_TEST(test_str);
	RUN_TEST(test_add_scalar);
	RUN_TEST(test_add_matrix);
	RUN_TEST(test_sub_scalar);
	RUN_TEST(test_sub_matrix);
	RUN_TEST(test_mul_scalar);
	RUN_TEST(test_mul_matrix);
	RUN_TEST(test_div_scalar);
	RUN_TEST(test_div_matrix);
	RUN_TEST(test_mod_scalar);
	RUN_TEST(test_mod_matrix);
	RUN_TEST(test_neg);
	RUN_TEST(test_pow);
	RUN_TEST(test_equal_true);
	RUN_TEST(test_equal_false);
	RUN_TEST(test_promote_rational);
	RUN_TEST(test_promote_complex);
	RUN_TEST(test_promote_vector);
	RUN_TEST(test_promote_matrix);
	RUN_TEST(test_eye);
	RUN_TEST(test_mmul);
	RUN_TEST(test_transpose);
	RUN_TEST(test_solve);
	RUN_TEST(test_inverse);
	RUN_TEST(test_determinant);
	return UNITY_END();
}
