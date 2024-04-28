#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"
#include "Str.h"
#include "UnorderedMap.h"
#include "UnorderedMap.r"
#include "Vec.h"

void setUp(void) {
	initStr();
	initUnorderedMap();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_UnorderedMap	*m = new(UnorderedMap);

	TEST_ASSERT_NOT_NULL(m);
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 0);
	delete(m);
}

void test_copy(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);
	struct s_UnorderedMap		*m_copy;

	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	m_copy = copy(m);
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), UnorderedMap_size(m_copy));
	void	*val;
	void	*val_copy;
	val = UnorderedMap_find(m, str0);
	val_copy = UnorderedMap_find(m_copy, str0);
	TEST_ASSERT_TRUE(equal(val, val_copy));
	val = UnorderedMap_find(m, str1);
	val_copy = UnorderedMap_find(m_copy, str1);
	TEST_ASSERT_TRUE(equal(val, val_copy));
	delete(m);
	delete(m_copy);
}

void test_empty(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);

	TEST_ASSERT_TRUE(UnorderedMap_empty(m));
	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_FALSE(UnorderedMap_empty(m));
	delete(m);
}

void test_size(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);

	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 0);
	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 2);
	delete(m);
}

void test_find(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);

	UnorderedMap_insert(m, str0, copy(str0));
	TEST_ASSERT_TRUE(equal(UnorderedMap_find(m, str0), str0));
	TEST_ASSERT_NOT_EQUAL(UnorderedMap_find(m, str1), str1);
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_TRUE(equal(UnorderedMap_find(m, str1), str1));
	delete(m);
}

void test_insert(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);

	UnorderedMap_insert(m, str0, str1);
	TEST_ASSERT_TRUE(equal(UnorderedMap_find(m, str0), str1));
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 1);
	delete(m);
}

void test_erase(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);

	UnorderedMap_insert(m, str0, str1);
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 1);
	TEST_ASSERT_EQUAL(UnorderedMap_erase(m, str1), 0);
	TEST_ASSERT_EQUAL(UnorderedMap_erase(m, str0), 1);
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 0);
	delete(m);
}

void test_clear(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);

	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 2);
	UnorderedMap_clear(m);
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 0);
	delete(m);
}

void test_reserve(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap);

	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(m->key), 0);
	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(m->key), 2);
	UnorderedMap_reserve(m, 5);
	TEST_ASSERT_GREATER_OR_EQUAL(Vec_capacity(m->key), 5);
	delete(m);
}

void test_swap(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m0 = new(UnorderedMap);
	struct s_UnorderedMap		*m1 = new(UnorderedMap);
	struct s_UnorderedMap		*m0_copy;
	struct s_UnorderedMap		*m1_copy;

	UnorderedMap_insert(m0, str0, copy(str0));
	UnorderedMap_insert(m0, str1, copy(str1));
	m0_copy = copy(m0);
	m1_copy = copy(m1);

	swap_UnorderedMap(m0, m1);
	TEST_ASSERT_TRUE(equal(m0_copy, m1));
	TEST_ASSERT_TRUE(equal(m1_copy, m0));
	delete(m0);
	delete(m1);
	delete(m0_copy);
	delete(m1_copy);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_ctor);
	RUN_TEST(test_copy);
	RUN_TEST(test_empty);
	RUN_TEST(test_size);
	RUN_TEST(test_find);
	RUN_TEST(test_insert);
	RUN_TEST(test_erase);
	RUN_TEST(test_clear);
	RUN_TEST(test_reserve);
	RUN_TEST(test_swap);
	return UNITY_END();
}
