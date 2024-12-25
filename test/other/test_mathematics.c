#include <math.h>

#include "unity.h"

/* container */
#include "Vec.h"

/* lexer */
#include "Complex.h"
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

	for (v = -10; v < 10; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_exp_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = exp(v);

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
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

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
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

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_sin_Rational(void) {
	double	v;

	for (v = -10; v < 10; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_sin_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = sin(v);

		if (target < -1e-15 || target > 1e-15 )
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_sin_Complex(void) {
	double	a;
	double	b;

	for (a = -5; a < 5; a += 0.1)
	{
		for (b = -5; b < 5; b += 0.1)
		{
			void	*z = new(Complex, COMPLEX,
							Rational_from_double(a),
							Rational_from_double(b));
			void	*sin_z = ft_sin_Complex(z);
			void	*sin_z_real = Complex_real(sin_z);
			void	*sin_z_imag = Complex_imag(sin_z);
			double	calc_real = Rational_to_double(sin_z_real);
			double	calc_imag = Rational_to_double(sin_z_imag);
			double	real = sin(a) * cosh(b);
			double	imag = cos(a) * sinh(b);

			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_real, real);
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_imag, imag);
			delete(z);
			delete(sin_z);
			delete(sin_z_real);
			delete(sin_z_imag);
		}
	}
}

void test_cos_Rational(void) {
	double	v;

	for (v = -10; v < 10; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_cos_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = cos(v);

		if (target < -1e-15 || target > 1e-15 )
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_cos_Complex(void) {
	double	a;
	double	b;

	for (a = -5; a < 5; a += 0.1)
	{
		for (b = -5; b < 5; b += 0.1)
		{
			void	*z = new(Complex, COMPLEX,
							Rational_from_double(a),
							Rational_from_double(b));
			void	*cos_z = ft_cos_Complex(z);
			void	*cos_z_real = Complex_real(cos_z);
			void	*cos_z_imag = Complex_imag(cos_z);
			double	calc_real = Rational_to_double(cos_z_real);
			double	calc_imag = Rational_to_double(cos_z_imag);
			double	real = cos(a) * cosh(b);
			double	imag = -sin(a) * sinh(b);

			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_real, real);
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_imag, imag);
			delete(z);
			delete(cos_z);
			delete(cos_z_real);
			delete(cos_z_imag);
		}
	}
}

void test_tan_Rational(void) {
	double	v;

	for (v = -10; v < 10; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_tan_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = tan(v);

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_tan_Complex(void) {
	double	a;
	double	b;

	for (a = -1.5; a < 1.5; a += 0.1)
	{
		for (b = -1.5; b < 1.5; b += 0.1)
		{
			void	*z = new(Complex, COMPLEX,
							Rational_from_double(a),
							Rational_from_double(b));
			void	*tan_z = ft_tan_Complex(z);
			void	*tan_z_real = Complex_real(tan_z);
			void	*tan_z_imag = Complex_imag(tan_z);
			double	calc_real = Rational_to_double(tan_z_real);
			double	calc_imag = Rational_to_double(tan_z_imag);
			double	real = sin(2*a) / (cos(2*a) + cosh(2*b));
			double	imag = sinh(2*b) / (cos(2*a) + cosh(2*b));

			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_real, real);
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_imag, imag);
			delete(z);
			delete(tan_z);
			delete(tan_z_real);
			delete(tan_z_imag);
		}
	}
}

void test_sinh_Rational(void) {
	double	v;

	for (v = -10; v < 1; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_sinh_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = sinh(v);

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_sinh_Complex(void) {
	double	a;
	double	b;

	for (a = -5; a < 5; a += 0.1)
	{
		for (b = -5; b < 5; b += 0.1)
		{
			void	*z = new(Complex, COMPLEX,
							Rational_from_double(a),
							Rational_from_double(b));
			void	*sinh_z = ft_sinh_Complex(z);
			void	*sinh_z_real = Complex_real(sinh_z);
			void	*sinh_z_imag = Complex_imag(sinh_z);
			double	calc_real = Rational_to_double(sinh_z_real);
			double	calc_imag = Rational_to_double(sinh_z_imag);
			double	real = sinh(a) * cos(b);
			double	imag = cosh(a) * sin(b);

			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_real, real);
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_imag, imag);
			delete(z);
			delete(sinh_z);
			delete(sinh_z_real);
			delete(sinh_z_imag);
		}
	}
}

void test_cosh_Rational(void) {
	double	v;

	for (v = -10; v < 10; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_cosh_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = cosh(v);

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_cosh_Complex(void) {
	double	a;
	double	b;

	for (a = -5; a < 5; a += 0.1)
	{
		for (b = -5; b < 5; b += 0.1)
		{
			void	*z = new(Complex, COMPLEX,
							Rational_from_double(a),
							Rational_from_double(b));
			void	*cosh_z = ft_cosh_Complex(z);
			void	*cosh_z_real = Complex_real(cosh_z);
			void	*cosh_z_imag = Complex_imag(cosh_z);
			double	calc_real = Rational_to_double(cosh_z_real);
			double	calc_imag = Rational_to_double(cosh_z_imag);
			double	real = cosh(a) * cos(b);
			double	imag = sinh(a) * sin(b);

			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_real, real);
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_imag, imag);
			delete(z);
			delete(cosh_z);
			delete(cosh_z_real);
			delete(cosh_z_imag);
		}
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

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_tanh_Complex(void) {
	double	a;
	double	b;

	for (a = -1.5; a < 1.5; a += 0.1)
	{
		for (b = -1.5; b < 1.5; b += 0.1)
		{
			void	*z = new(Complex, COMPLEX,
							Rational_from_double(a),
							Rational_from_double(b));
			void	*tanh_z = ft_tanh_Complex(z);
			void	*tanh_z_real = Complex_real(tanh_z);
			void	*tanh_z_imag = Complex_imag(tanh_z);
			double	calc_real = Rational_to_double(tanh_z_real);
			double	calc_imag = Rational_to_double(tanh_z_imag);
			double	real = sinh(2*a) / (cosh(2*a) + cos(2*b));
			double	imag = sin(2*b) / (cosh(2*a) + cos(2*b));

			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_real, real);
			TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_imag, imag);
			delete(z);
			delete(tanh_z);
			delete(tanh_z_real);
			delete(tanh_z_imag);
		}
	}
}

void test_sqrt_Rational(void)
{
	double	v;

	for (v = 0.1; v < 5; v += 0.1)
	{
		void	*r_in = Rational_from_double(v);
		void	*r_out = ft_sqrt_Rational(r_in);
		double	calc = Rational_to_double(r_out);
		double	target = sqrt(v);

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_sqrt_Complex(void) {
	double	a;
	double	b;

	for (a = -5; a < 5; a += 0.1)
	{
		for (b = -5; b < 5; b += 0.1)
		{
			void	*z = new(Complex, COMPLEX,
							Rational_from_double(a),
							Rational_from_double(b));
			void	*sqrt_z = ft_sqrt_Complex(z);
			void	*sqrt_z_real = Complex_real(sqrt_z);
			void	*sqrt_z_imag = Complex_imag(sqrt_z);
			double	calc_real = Rational_to_double(sqrt_z_real);
			double	calc_imag = Rational_to_double(sqrt_z_imag);
			double	r = sqrt(sqrt(a*a + b*b));
			double	theta = atan2(b, a) / 2;
			double	real = r * cos(theta);
			double	imag = r * sin(theta);

			/* due to floating point error b is not exactly 0 */
			/* when b is nearly 0, output is sensitive to the sign of b */
			if (fabs(b) > __FLT_EPSILON__)
			{
				TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_real, real);
				TEST_ASSERT_FLOAT_WITHIN(1e-15, calc_imag, imag);
			}
			delete(z);
			delete(sqrt_z);
			delete(sqrt_z_real);
			delete(sqrt_z_imag);
		}
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

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
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

		TEST_ASSERT_FLOAT_WITHIN(1e-15, calc, target);
		delete(r_in);
		delete(r_out);
		(void)calc;
		(void)target;
	}
}

void test_norm_Matrix(void) {
	struct s_Rational	*r00 = Rational_from_long((long)-3, 1);
	struct s_Rational	*r01 = Rational_from_long(5, 1);
	struct s_Rational	*r02 = Rational_from_long(7, 1);
	struct s_Rational	*r10 = Rational_from_long(2, 1);
	struct s_Rational	*r11 = Rational_from_long(6, 1);
	struct s_Rational	*r12 = Rational_from_long(4, 1);
	struct s_Rational	*r20 = Rational_from_long(0, 1);
	struct s_Rational	*r21 = Rational_from_long(2, 1);
	struct s_Rational	*r22 = Rational_from_long(8, 1);
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
	TEST_ASSERT_FLOAT_WITHIN(1e-15, r_double, 19);
	delete(m);
	delete(r);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_exp_Rational);
	RUN_TEST(test_ln_Rational);
	RUN_TEST(test_sin_Rational);
	RUN_TEST(test_sin_Complex);
	RUN_TEST(test_cos_Rational);
	RUN_TEST(test_cos_Complex);
	RUN_TEST(test_tan_Rational);
	RUN_TEST(test_tan_Complex);
	RUN_TEST(test_sinh_Rational);
	RUN_TEST(test_sinh_Complex);
	RUN_TEST(test_cosh_Rational);
	RUN_TEST(test_cosh_Complex);
	RUN_TEST(test_tanh_Rational);
	RUN_TEST(test_tanh_Complex);
	RUN_TEST(test_sqrt_Rational);
	RUN_TEST(test_sqrt_Complex);
	RUN_TEST(test_radians_Rational);
	RUN_TEST(test_degrees_Rational);
	RUN_TEST(test_norm_Matrix);
	return UNITY_END();
}
