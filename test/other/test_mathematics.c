#include <math.h>

#include "unity.h"

/* container */
#include "Vec.h"

/* lexer */
#include "Rational.h"
#include "Matrix.h"

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

void test_sinh_Rational(void) {
	double	v;

	for (v = -3; v < 1; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_sinh_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = sinh(v);

		if (target < - 0.001 || target > 0.001 )
			TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_cosh_Rational(void) {
	double	v;

	for (v = -3; v < 3; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_cosh_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = cosh(v);

		if (target < - 0.001 || target > 0.001 )
			TEST_ASSERT_FLOAT_WITHIN(0.01, 1, calc / target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_tanh_Rational(void) {
	double	v;

	for (v = -3; v < 3; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_tanh_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = tanh(v);

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

void test_norm_Matrix(void) {
	struct s_Rational	*r00 = new(Rational, RATIONAL, (long)-3, 1);
	struct s_Rational	*r01 = new(Rational, RATIONAL, 5, 1);
	struct s_Rational	*r02 = new(Rational, RATIONAL, 7, 1);
	struct s_Rational	*r10 = new(Rational, RATIONAL, 2, 1);
	struct s_Rational	*r11 = new(Rational, RATIONAL, 6, 1);
	struct s_Rational	*r12 = new(Rational, RATIONAL, 4, 1);
	struct s_Rational	*r20 = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*r21 = new(Rational, RATIONAL, 2, 1);
	struct s_Rational	*r22 = new(Rational, RATIONAL, 8, 1);
	void				*v0 = new(Vec);
	void				*v1 = new(Vec);
	void				*v2 = new(Vec);
	void				*v = new(Vec);
	struct s_Matrix		*m;
	void				*r;
	double				r_double;

	Vec_push_back(v0, r00);
	Vec_push_back(v0, r01);
	Vec_push_back(v0, r02);
	Vec_push_back(v1, r10);
	Vec_push_back(v1, r11);
	Vec_push_back(v1, r12);
	Vec_push_back(v2, r20);
	Vec_push_back(v2, r21);
	Vec_push_back(v2, r22);
	Vec_push_back(v, v0);
	Vec_push_back(v, v1);
	Vec_push_back(v, v2);
	m = new(Matrix, MATRIX, v);

	r = ft_norm_Matrix(m);
	r_double = Rational_to_double(r);
	TEST_ASSERT_FLOAT_WITHIN(0.01, 1, r_double / 19);
	delete(m);
	delete(r);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_exp_Rational);
	RUN_TEST(test_ln_Rational);
	RUN_TEST(test_sin_Rational);
	RUN_TEST(test_cos_Rational);
	RUN_TEST(test_tan_Rational);
	RUN_TEST(test_sinh_Rational);
	RUN_TEST(test_cosh_Rational);
	RUN_TEST(test_tanh_Rational);
	RUN_TEST(test_radians_Rational);
	RUN_TEST(test_degrees_Rational);
	RUN_TEST(test_norm_Matrix);
	return UNITY_END();
}
