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
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 7, 8);
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
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 7, 8);
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
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	TEST_ASSERT_NULL(numeric_add(m0, r));
	delete(m0);
	delete(r);
}

void test_add_matrix(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 3, 2);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 5, 3);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 7, 4);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_add(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_sub_scalar(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	TEST_ASSERT_NULL(numeric_sub(m0, r));
	delete(m0);
	delete(r);
}

void test_sub_matrix(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 0, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_sub(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_mul_scalar(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);

	struct s_Rational	*r10 = new(Rational, RATIONAL, 1, 4);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 3, 8);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 5, 12);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 7, 16);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_mul(m0, r);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(r);
	delete(m);
}

void test_mul_matrix(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, 1, 4);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 9, 16);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 25, 36);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 49, 64);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_mul(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_div_scalar(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);

	struct s_Rational	*r10 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 3, 2);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 5, 3);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 7, 4);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_div(m0, r);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(r);
	delete(m);
}

void test_div_matrix(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 1, 1);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_div(m0, m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_mod_scalar(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);

	struct s_Rational	*r10 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 1, 4);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 1, 3);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 3, 8);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_mod(m0, r);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(r);
	delete(m);
}

void test_mod_matrix(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	m = numeric_mod(m0, m0);
	TEST_ASSERT_NULL(m);
	delete(m0);
}

void test_neg(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, (long)-1, 2);
	struct s_Rational	*r11 = new(Rational, RATIONAL, (long)-3, 4);
	struct s_Rational	*r12 = new(Rational, RATIONAL, (long)-5, 6);
	struct s_Rational	*r13 = new(Rational, RATIONAL, (long)-7, 8);
	void				*v10 = new(Vec);
	void				*v11 = new(Vec);
	void				*v1 = new(Vec);
	struct s_Matrix		*m1;

	struct s_Matrix		*m;

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	Vec_push_back(v10, r10);
	Vec_push_back(v10, r11);
	Vec_push_back(v11, r12);
	Vec_push_back(v11, r13);
	Vec_push_back(v1, new(Vector, VECTOR, v10));
	Vec_push_back(v1, new(Vector, VECTOR, v11));
	m1 = new(Matrix, MATRIX, v1);

	m = numeric_neg(m0);
	TEST_ASSERT_TRUE(numeric_equal(m, m1));
	delete(m0);
	delete(m1);
	delete(m);
}

void test_pow(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r = new(Rational, RATIONAL, 1, 2);

	Vec_push_back(v00, r00);
	Vec_push_back(v00, r01);
	Vec_push_back(v01, r02);
	Vec_push_back(v01, r03);
	Vec_push_back(v0, new(Vector, VECTOR, v00));
	Vec_push_back(v0, new(Vector, VECTOR, v01));
	m0 = new(Matrix, MATRIX, v0);

	TEST_ASSERT_NULL(numeric_pow(m0, r));
	delete(m0);
	delete(r);
}

void test_equal_true(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 7, 8);
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
	struct s_Rational	*r00 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r03 = new(Rational, RATIONAL, 7, 8);
	void				*v00 = new(Vec);
	void				*v01 = new(Vec);
	void				*v0 = new(Vec);
	struct s_Matrix		*m0;

	struct s_Rational	*r10 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r13 = new(Rational, RATIONAL, 7, 9);
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
	// RUN_TEST(test_pow);
	RUN_TEST(test_equal_true);
	RUN_TEST(test_equal_false);
	return UNITY_END();
}