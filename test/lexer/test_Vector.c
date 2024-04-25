#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"

/* container */
#include "Vec.h"

/* lexer */
#include "Numeric.h"
#include "Rational.h"
#include "Vector.h"

void setUp(void) {
	initRational();
	initVector();
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
	struct s_Rational	*r2 = new(Rational, RATIONAL, 7, 2);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 21, 4);
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
	return UNITY_END();
}
