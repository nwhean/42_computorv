#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"
#include "Rational.h"
#include "Vec.h"
#include "Vec.r"

void setUp(void) {
	initRational();
	initVec();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_Vec	*v = new(Vec);

	TEST_ASSERT_NOT_NULL(v);
	TEST_ASSERT_EQUAL(v->size, 0);
	TEST_ASSERT_EQUAL(v->capacity, 0);
	delete(v);
}

void test_copy(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);
	struct s_Vec		*v_copy;
	size_t				i;

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	v_copy = copy(v);
	TEST_ASSERT_EQUAL(v->size, v_copy->size);
	for (i = 0; i < v->size; ++i)
	{
		struct s_Rational	*v_val = v->data[i];
		struct s_Rational	*v_copy_val = v_copy->data[i];
		TEST_ASSERT_TRUE(numeric_equal(v_val, v_copy_val));
	}
	delete(v);
	delete(v_copy);
}

void test_str(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 2, 1);
	struct s_Vec		*v = new(Vec);
	char				*s;

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	s = str(v);
	TEST_ASSERT_EQUAL_STRING_LEN(s, "[1, 2]", 6);
	free(s);
	delete(v);
}

void test_equal_true(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_TRUE(equal(v, v));
	delete(v);
}

void test_equal_false(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v0 = new(Vec);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Rational	*r3 = new(Rational, RATIONAL, 7, 8);
	struct s_Vec		*v1 = new(Vec);

	Vec_push_back(v0, r0);
	Vec_push_back(v0, r1);
	Vec_push_back(v1, r2);
	Vec_push_back(v1, r3);
	TEST_ASSERT_FALSE(equal(v0, v1));
	delete(v0);
	delete(v1);
}

void test_size(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_EQUAL(Vec_size(v), 2);
	delete(v);
}

void test_capacity(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(v), 0);
	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(v), 2);
	delete(v);
}

void test_empty(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	TEST_ASSERT_TRUE(Vec_empty(v));
	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_FALSE(Vec_empty(v));
	delete(v);
}

void test_reserve(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(v), 0);
	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(v), 2);
	Vec_reserve(v, 5);
	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(v), 5);
	delete(v);
}

void test_at(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_TRUE(numeric_equal(Vec_at(v, 0), r0));
	TEST_ASSERT_TRUE(numeric_equal(Vec_at(v, 1), r1));
	delete(v);
}

void test_front(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Vec		*v = new(Vec);

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	Vec_push_back(v, r2);
	TEST_ASSERT_TRUE(numeric_equal(Vec_front(v), r0));
	delete(v);
}

void test_back(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Vec		*v = new(Vec);

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	Vec_push_back(v, r2);
	TEST_ASSERT_TRUE(numeric_equal(Vec_back(v), r2));
	delete(v);
}

void test_data(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Vec		*v = new(Vec);

	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	Vec_push_back(v, r2);
	TEST_ASSERT_EQUAL(v->data, Vec_data(v));
	delete(v);
}

void test_push_back(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Vec		*v = new(Vec);

	TEST_ASSERT_EQUAL(v->size, 0);
	Vec_push_back(v, r0);
	TEST_ASSERT_EQUAL(v->size, 1);
	TEST_ASSERT_TRUE(numeric_equal(v->data[0], r0));
	delete(v);
}

void test_pop_back(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	TEST_ASSERT_EQUAL(v->size, 0);
	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_EQUAL(v->size, 2);
	Vec_pop_back(v);
	TEST_ASSERT_EQUAL(v->size, 1);
	TEST_ASSERT_TRUE(numeric_equal(v->data[0], r0));
	delete(v);
}

void test_erase(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Rational	*r2 = new(Rational, RATIONAL, 5, 6);
	struct s_Vec		*v = new(Vec);

	TEST_ASSERT_EQUAL(v->size, 0);
	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	Vec_push_back(v, r2);
	TEST_ASSERT_EQUAL(v->size, 3);
	Vec_erase(v, 1);
	TEST_ASSERT_EQUAL(v->size, 2);
	TEST_ASSERT_TRUE(numeric_equal(v->data[0], r0));
	TEST_ASSERT_TRUE(numeric_equal(v->data[1], r2));
	TEST_ASSERT_EQUAL(v->data[2], NULL);
	delete(v);
}

void test_clear(void) {
	struct s_Rational	*r0 = new(Rational, RATIONAL, 1, 2);
	struct s_Rational	*r1 = new(Rational, RATIONAL, 3, 4);
	struct s_Vec		*v = new(Vec);

	TEST_ASSERT_EQUAL(v->size, 0);
	Vec_push_back(v, r0);
	Vec_push_back(v, r1);
	TEST_ASSERT_EQUAL(v->size, 2);
	Vec_clear(v);
	TEST_ASSERT_EQUAL(v->size, 0);
	delete(v);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_ctor);
	RUN_TEST(test_copy);
	RUN_TEST(test_str);
	RUN_TEST(test_equal_true);
	RUN_TEST(test_equal_false);
	RUN_TEST(test_size);
	RUN_TEST(test_capacity);
	RUN_TEST(test_empty);
	RUN_TEST(test_reserve);
	RUN_TEST(test_at);
	RUN_TEST(test_front);
	RUN_TEST(test_back);
	RUN_TEST(test_data);
	RUN_TEST(test_push_back);
	RUN_TEST(test_pop_back);
	RUN_TEST(test_erase);
	RUN_TEST(test_clear);
	return UNITY_END();
}
