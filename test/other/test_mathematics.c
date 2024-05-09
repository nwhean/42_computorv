#include <math.h>

#include "unity.h"

/* lexer */
#include "Rational.h"

/* other */
#include "mathematics.h"
#include "mathematics.r"

void setUp(void) {
	initRational();
}

void tearDown(void) {
}

void test_exp_Rational(void) {
	double	v;

	for (v = -16; v < 16; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_exp_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = exp(v);

		TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_ln_Rational(void) {
	double	v;

	for (v = 0.999; v > 0.5; v -= 0.01)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_ln_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = log(v);

		TEST_ASSERT_FLOAT_WITHIN(0.05, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}

	for (v = 1.1; v < 1000; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_ln_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = log(v);

		TEST_ASSERT_FLOAT_WITHIN(0.03, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_sin_Rational(void) {
	double	v;

	for (v = -16; v < 16; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_sin_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = sin(v);

		if (target < - 0.001 || target > 0.001 )
			TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_cos_Rational(void) {
	double	v;

	for (v = -16; v < 16; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_cos_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = cos(v);

		if (target < - 0.001 || target > 0.001 )
			TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_tan_Rational(void) {
	double	v;

	for (v = -16; v < 16; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_tan_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = tan(v);

		if (target < - 0.001 || target > 0.001 )
			TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_radians_Rational(void) {
	double	v;

	for (v = -180; v < 180; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_radians_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = v / 180 * 3.141592653589793;

		if (target < - 0.001 || target > 0.001 )
			TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_degrees_Rational(void) {
	double	v;

	for (v = -3.141592653589793; v < 3.141592653589793; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_degrees_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = v * 180 / 3.141592653589793;

		if (target < - 0.001 || target > 0.001 )
			TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_exp_Rational);
	RUN_TEST(test_ln_Rational);
	RUN_TEST(test_sin_Rational);
	RUN_TEST(test_cos_Rational);
	RUN_TEST(test_tan_Rational);
	RUN_TEST(test_radians_Rational);
	RUN_TEST(test_degrees_Rational);
	return UNITY_END();
}
