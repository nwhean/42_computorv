#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "unity.h"
#include "Str.h"
#include "UnorderedMap.h"
#include "UnorderedMap.r"
#include "Rational.h"

void setUp(void) {
	initStr();
	initUnorderedMap();
}

void tearDown(void) {
}

void test_ctor(void) {
	struct s_UnorderedMap	*m = new(UnorderedMap, Str_compare);

	TEST_ASSERT_NOT_NULL(m);
	TEST_ASSERT_EQUAL(m->size, 0);
	TEST_ASSERT_EQUAL(m->capacity, 0);
	delete(m);
}

void test_copy(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);
	struct s_UnorderedMap		*m_copy;

	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	m_copy = copy(m);
	TEST_ASSERT_EQUAL(m->size, m_copy->size);
	void	*val;
	void	*val_copy;
	val = UnorderedMap_find(m, str0);
	val_copy = UnorderedMap_find(m_copy, str0);
	TEST_ASSERT_TRUE(Str_compare(val, val_copy) == 0);
	val = UnorderedMap_find(m, str1);
	val_copy = UnorderedMap_find(m_copy, str1);
	TEST_ASSERT_TRUE(Str_compare(val, val_copy) == 0);
	delete(m);
	delete(m_copy);
}

void test_empty(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);

	TEST_ASSERT_TRUE(UnorderedMap_empty(m));
	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_FALSE(UnorderedMap_empty(m));
	delete(m);
}

void test_size(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);

	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 0);
	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_EQUAL(UnorderedMap_size(m), 2);
	delete(m);
}

void test_find(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);

	UnorderedMap_insert(m, str0, copy(str0));
	TEST_ASSERT_TRUE(Str_compare(UnorderedMap_find(m, str0), str0) == 0);
	TEST_ASSERT_NOT_EQUAL(UnorderedMap_find(m, str1), str1);
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_TRUE(Str_compare(UnorderedMap_find(m, str1), str1) == 0);
	delete(m);
}

void test_insert(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);

	UnorderedMap_insert(m, str0, str1);
	TEST_ASSERT_TRUE(Str_compare(UnorderedMap_find(m, str0), str1) == 0);
	TEST_ASSERT_EQUAL(m->size, 1);
	delete(m);
}

void test_erase(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);

	UnorderedMap_insert(m, str0, str1);
	TEST_ASSERT_EQUAL(m->size, 1);
	TEST_ASSERT_EQUAL(UnorderedMap_erase(m, str1), 0);
	TEST_ASSERT_EQUAL(UnorderedMap_erase(m, str0), 1);
	TEST_ASSERT_EQUAL(m->size, 0);
	delete(m);
}

void test_clear(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);

	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_EQUAL(m->size, 2);
	UnorderedMap_clear(m);
	TEST_ASSERT_EQUAL(m->size, 0);
	delete(m);
}

void test_reserve(void) {
	void						*str0 = new(Str, "Hello");
	void						*str1 = new(Str, "World");
	struct s_UnorderedMap		*m = new(UnorderedMap, Str_compare);

	TEST_ASSERT_GREATER_OR_EQUAL(m->capacity, 0);
	UnorderedMap_insert(m, str0, copy(str0));
	UnorderedMap_insert(m, str1, copy(str1));
	TEST_ASSERT_GREATER_OR_EQUAL(m->capacity, 2);
	UnorderedMap_reserve(m, 5);
	TEST_ASSERT_GREATER_OR_EQUAL(m->capacity, 5);
	delete(m);
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
	return UNITY_END();
}
