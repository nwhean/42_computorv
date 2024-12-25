/* container */
#include "Vec.h"

/* inter */
#include "Expr.h"

/* lexer */
#include "Complex.h"
#include "Matrix.h"
#include "Rational.h"
#include "Vector.h"

/* other */
#include "mathematics.h"
#include "mathematics.r"

size_t	max(size_t a, size_t b)
{
	return (a > b ? a : b);
}

/* Return the exponentiation of a Rational number
 * In order to preserve the precision of the algorithm, floating point
 * number is used in the algorithm.
 */
struct s_Rational	*ft_exp_Rational(struct s_Rational *_x)
{
	mpf_t			x;
	mpf_t			x_den;
	mpf_t			sum;
	mpf_t			reldiff;
	mpf_t			x1;
	mpq_t			rational;
	mpz_t			num;
	mpz_t			den;
	void			*retval;
	int				i = 0;
	unsigned long	n;

	/* initialise values */
	mpf_init(x);
	mpf_init(x_den);
	mpf_init(sum);
	mpf_init(reldiff);
	mpf_init(x1);
	mpq_init(rational);
	mpz_init(num);
	mpz_init(den);
	mpf_set_z(x, _x->numerator);
	mpf_set_z(x_den, _x->denominator);
	mpf_div(x, x, x_den);

	/* scale until -1 < x < 1 */
	while (mpf_cmp_si(x, -1) < 0 || mpf_cmp_si(x, 1) > 0)
	{
		mpf_div_ui(x, x, 2);
		i++;
	}

	/* exp(x) = 1 + x + x^2 / 2! + x^3 / 3! + x^4 / 4! + ... */
	n = 0;
	mpf_set_ui(sum, 0);					/* sum = 0 */
	mpf_set_ui(x1, 1);					/* x1 = 1 */
	do {
		mpf_set(reldiff, sum);			/* save a copy of sum */
		mpf_add(sum, sum, x1);			/* sum += x1 */
		mpf_mul(x1, x1, x);				/* x1 = x1 * x/n */
		mpf_div_ui(x1, x1, ++n);
		mpf_reldiff(reldiff, sum, reldiff);	/* calculate relative difference */
	} while (mpf_cmp_d(reldiff, 1e-15) > 0);

	/* square until i = 0 */
	while (i--)
		mpf_mul(sum, sum, sum);

	/* convert value to rational */
	mpq_set_f(rational, sum);
	mpq_get_num(num, rational);
	mpq_get_den(den, rational);
	retval = new(Rational, RATIONAL, num, den);

	/* clear values */
	mpf_clear(x);
	mpf_clear(x_den);
	mpf_clear(sum);
	mpf_clear(reldiff);
	mpf_clear(x1);
	mpq_clear(rational);
	mpz_clear(num);
	mpz_clear(den);

	return (retval);
}

/* Return the exponentiation of a Complex number */
struct s_Complex	*ft_exp_Complex(struct s_Complex *x)
{
	void	*pi = Rational_from_double(3.14159265358979323846);
	void	*a = Complex_real(x);
	void	*b = Complex_imag(x);
	void	*mod = ft_exp_Rational(a);
	void	*retval;

	/* shift b so that -pi < b < pi*/
	while (Rational_gt(b, pi))
	{
		numeric_isub(&b, pi);
		numeric_isub(&b, pi);
	}
	numeric_ineg(&pi);
	while (Rational_lt(b, pi))
	{
		numeric_isub(&b, pi);
		numeric_isub(&b, pi);
	}

	retval = Complex_from_polar(mod, b);
	delete(pi);
	delete(a);
	delete(b);
	delete(mod);
	return (retval);
}

/* Return the exponentiation of a number. */
void	*ft_exp(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_exp_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_exp_Complex(x);
			break ;
		default:
			fprintf(stderr, "exp function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the natural logarithm of a Rational number */
struct s_Rational	*ft_ln_Rational(struct s_Rational *_x)
{
	mpf_t			x;
	mpf_t			x_den;
	mpf_t			sum;
	mpf_t			reldiff;
	mpf_t			term;
	mpf_t			x1;
	mpf_t			ln2;
	mpq_t			rational;
	mpz_t			num;
	mpz_t			den;
	void			*retval;
	int				i = 0;
	unsigned long	n;

	if (mpz_cmp_ui(_x->numerator, 0) <= 0)
	{
		fprintf(stderr, "ft_ln_Rational: domain error.\n");
		return (NULL);
	}

	/* initialise values */
	mpf_init(x);
	mpf_init(x_den);
	mpf_init(sum);
	mpf_init(reldiff);
	mpf_init(term);
	mpf_init(x1);
	mpq_init(rational);
	mpz_init(num);
	mpz_init(den);
	mpf_set_z(x, _x->numerator);
	mpf_set_z(x_den, _x->denominator);
	mpf_div(x, x, x_den);

	/* scale until 0 < x < 1 */
	while (mpf_cmp_si(x, 1) > 0)
	{
		mpf_div_ui(x, x, 2);
		i++;
	}

	/* ln(x) = -(1-x) - (1-x)^2 / 2 - (1-x)^3 / 3 - (1-x)^4 / 4 + ... */
	n = 1;
	mpf_set_ui(sum, 0);					/* sum = 0 */
	mpf_sub_ui(x, x, 1);				/* x = 1 - x */
	mpf_neg(x, x);
	mpf_set(x1, x);						/* x1 = x */
	do {
		mpf_set(reldiff, sum);			/* save a copy of sum */
		mpf_div_ui(term, x1, n++);		/* term = x1 / n */
		mpf_sub(sum, sum, term);		/* sum -= term */
		mpf_mul(x1, x1, x);				/* x1 = x1 * x */
		mpf_reldiff(reldiff, sum, reldiff);	/* calculate relative difference */
		mpf_abs(reldiff, reldiff);
	} while (mpf_cmp_d(reldiff, 1e-15) > 0);

	/* sum = sum + i * ln(2) */
	mpf_init_set_d(ln2, 0.6931471805599453);
	mpf_mul_ui(ln2, ln2, i);
	mpf_add(sum, sum, ln2);

	/* convert value to rational */
	mpq_set_f(rational, sum);
	mpq_get_num(num, rational);
	mpq_get_den(den, rational);
	retval = new(Rational, RATIONAL, num, den);

	/* clear values */
	mpf_clear(x);
	mpf_clear(x_den);
	mpf_clear(sum);
	mpf_clear(reldiff);
	mpf_clear(term);
	mpf_clear(x1);
	mpf_clear(ln2);
	mpq_clear(rational);
	mpz_clear(num);
	mpz_clear(den);

	return (retval);
}

/* Return the natural logarithm of a Complex number
 * ln(a + ib) = ln(re^(ix))
 *            = ln(r) + ln(e^(ix))
 *            = ln(r) + ix
 */
struct s_Complex	*ft_ln_Complex(struct s_Complex *x)
{
	void	*r = Complex_modulus(x);
	void	*theta = Complex_argument(x);
	void	*log_r = ft_ln_Rational(r);

	delete(r);
	return new(Complex, COMPLEX, log_r, theta);
}

/* Return the natural logarithm of a number. */
void	*ft_ln(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_ln_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_ln_Complex(x);
			break ;
		default:
			fprintf(stderr, "ln function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the sin of a Rational number */
struct s_Rational	*ft_sin_Rational(struct s_Rational *_x)
{
	mpf_t				x;
	mpf_t				x_den;
	mpf_t				sum;
	mpf_t				reldiff;
	mpf_t				x1;
	mpq_t				rational;
	mpz_t				num;
	mpz_t				den;
	struct s_Rational	*retval;
	unsigned long		n;

	/* initialise values */
	mpf_init(x);
	mpf_init(x_den);
	mpf_set_z(x, _x->numerator);
	mpf_set_z(x_den, _x->denominator);
	mpf_div(x, x, x_den);

	/* if x < -1 or x > 1, use double angle formula */
	while (mpf_cmp_si(x, -1) < 0 || mpf_cmp_si(x, 1) > 0)
	{
		struct s_Rational	*__x = copy(_x);
		struct s_Rational	*cos;
		struct s_Rational	*sin;

		/* sin(2x) = 2 sin(x) cos(x) */
		mpz_mul_ui(__x->denominator, __x->denominator, 2);
		cos = ft_cos_Rational(__x);
		sin = ft_sin_Rational(__x);
		retval = numeric_mul(cos, sin);
		if (mpz_even_p(retval->denominator))
			mpz_div_ui(retval->denominator, retval->denominator, 2);
		else
			mpz_mul_ui(retval->numerator, retval->numerator, 2);

		delete(__x);
		delete(cos);
		delete(sin);
		mpf_clear(x);
		mpf_clear(x_den);
		return (retval);
	}

	/* initialise values */
	mpf_init(sum);
	mpf_init(reldiff);
	mpf_init(x1);
	mpq_init(rational);
	mpz_init(num);
	mpz_init(den);

	/* sin(x) =  x - x^3 / 3! + x^5 / 5! - x^7 / 7! + ... */
	n = 1;
	mpf_set_ui(sum, 0);					/* sum = 0 */
	mpf_set(x1, x);						/* x1 = x */
	do {
		mpf_set(reldiff, sum);			/* save a copy of sum */
		mpf_add(sum, sum, x1);			/* sum += x1 */
		mpf_mul(x1, x1, x);				/* x1 = x1 * x/n */
		mpf_div_ui(x1, x1, ++n);
		mpf_mul(x1, x1, x);				/* x1 = -x1 * x/n */
		mpf_div_ui(x1, x1, ++n);
		mpf_neg(x1, x1);
		mpf_reldiff(reldiff, sum, reldiff);	/* calculate relative difference */
		mpf_abs(reldiff, reldiff);
	} while (mpf_cmp_d(reldiff, 1e-15) > 0);

	/* convert value to rational */
	mpq_set_f(rational, sum);
	mpq_get_num(num, rational);
	mpq_get_den(den, rational);
	retval = new(Rational, RATIONAL, num, den);

	/* clear values */
	mpf_clear(x);
	mpf_clear(x_den);
	mpf_clear(sum);
	mpf_clear(reldiff);
	mpf_clear(x1);
	mpq_clear(rational);
	mpz_clear(num);
	mpz_clear(den);

	return (retval);
}

/* Return the sin of a Complex number
 * sin(a + ib) = sin(a) cos(ib) + cos(a) sin(ib)
 *             = sin(a) cosh(b) + i cos(a) sinh(b)
 */
struct s_Complex	*ft_sin_Complex(struct s_Complex *x)
{
	void	*a = Complex_real(x);
	void	*b = Complex_imag(x);
	void	*sin_a = ft_sin_Rational(a);
	void	*cosh_b = ft_cosh_Rational(b);
	void	*cos_a = ft_cos_Rational(a);
	void	*sinh_b = ft_sinh_Rational(b);
	void	*real = numeric_mul(sin_a, cosh_b);
	void	*imag = numeric_mul(cos_a, sinh_b);

	delete(a);
	delete(b);
	delete(sin_a);
	delete(cosh_b);
	delete(cos_a);
	delete(sinh_b);
	return new(Complex, COMPLEX, real, imag);
}

/* Return the sin of a number. */
void	*ft_sin(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_sin_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_sin_Complex(x);
			break ;
		default:
			fprintf(stderr, "sin function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the cos of a Rational number */
struct s_Rational	*ft_cos_Rational(struct s_Rational *_x)
{
	mpf_t				x;
	mpf_t				x_den;
	mpf_t				sum;
	mpf_t				reldiff;
	mpf_t				x1;
	mpq_t				rational;
	mpz_t				num;
	mpz_t				den;
	struct s_Rational	*retval;
	unsigned long		n;

	/* initialise values */
	mpf_init(x);
	mpf_init(x_den);
	mpf_set_z(x, _x->numerator);
	mpf_set_z(x_den, _x->denominator);
	mpf_div(x, x, x_den);

	/* if x < -1 or x > 1, use double angle formula */
	while (mpf_cmp_si(x, -1) < 0 || mpf_cmp_si(x, 1) > 0)
	{
		struct s_Rational	*__x = copy(_x);
		struct s_Rational	*cos;

		/* cos(2x) = 2 cos(x)^2 - 1 */
		mpz_mul_ui(__x->denominator, __x->denominator, 2);
		cos = ft_cos_Rational(__x);
		retval = numeric_mul(cos, cos);		/* cos(x)^2 */

		/* multiply by 2 */
		if (mpz_even_p(retval->denominator))
			mpz_div_ui(retval->denominator, retval->denominator, 2);
		else
			mpz_mul_ui(retval->numerator, retval->numerator, 2);

		/* minus 1 */
		mpz_sub(retval->numerator, retval->numerator, retval->denominator);

		delete(__x);
		delete(cos);
		mpf_clear(x);
		mpf_clear(x_den);
		return (retval);
	}

	/* initialise values */
	mpf_init(sum);
	mpf_init(reldiff);
	mpf_init(x1);
	mpq_init(rational);
	mpz_init(num);
	mpz_init(den);

	/* cos(x) = 1 - x^2 / 2! + x^4 / 4! - x^6 / 6! + ... */
	n = 0;
	mpf_set_ui(sum, 0);					/* sum = 0 */
	mpf_set_ui(x1, 1);					/* x1 = 1 */
	do {
		mpf_set(reldiff, sum);			/* save a copy of sum */
		mpf_add(sum, sum, x1);			/* sum += x1 */
		mpf_mul(x1, x1, x);				/* x1 = x1 * x/n */
		mpf_div_ui(x1, x1, ++n);
		mpf_mul(x1, x1, x);				/* x1 = -x1 * x/n */
		mpf_div_ui(x1, x1, ++n);
		mpf_neg(x1, x1);
		mpf_reldiff(reldiff, sum, reldiff);	/* calculate relative difference */
		mpf_abs(reldiff, reldiff);
	} while (mpf_cmp_d(reldiff, 1e-15) > 0);

	/* convert value to rational */
	mpq_set_f(rational, sum);
	mpq_get_num(num, rational);
	mpq_get_den(den, rational);
	retval = new(Rational, RATIONAL, num, den);

	/* clear values */
	mpf_clear(x);
	mpf_clear(x_den);
	mpf_clear(sum);
	mpf_clear(reldiff);
	mpf_clear(x1);
	mpq_clear(rational);
	mpz_clear(num);
	mpz_clear(den);

	return (retval);
}

/* Return the cosine of a Complex number
 * cos(a + ib) = cos(a) cos(ib) - sin(a) sin(ib)
 *             = cos(a) cosh(b) - i sin(a) sinh(b)
 */
struct s_Complex	*ft_cos_Complex(struct s_Complex *x)
{
	void	*a = Complex_real(x);
	void	*b = Complex_imag(x);
	void	*cos_a = ft_cos_Rational(a);
	void	*cosh_b = ft_cosh_Rational(b);
	void	*sin_a = ft_sin_Rational(a);
	void	*sinh_b = ft_sinh_Rational(b);
	void	*real = numeric_mul(cos_a, cosh_b);
	void	*imag = numeric_mul(sin_a, sinh_b);

	numeric_ineg(&imag);
	delete(a);
	delete(b);
	delete(cos_a);
	delete(cosh_b);
	delete(sin_a);
	delete(sinh_b);
	return new(Complex, COMPLEX, real, imag);
}

/* Return the cos of a number. */
void	*ft_cos(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_cos_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_cos_Complex(x);
			break ;
		default:
			fprintf(stderr, "tan function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the tan of a Rational number */
struct s_Rational	*ft_tan_Rational(struct s_Rational *x)
{
	struct s_Rational	*sin = ft_sin_Rational(x);
	struct s_Rational	*cos = ft_cos_Rational(x);
	struct s_Rational	*retval = numeric_div(sin, cos);

	delete(sin);
	delete(cos);
	return (retval);
}

/* Return the tan of a Complex number */
struct s_Complex	*ft_tan_Complex(struct s_Complex *x)
{
	void	*sin = ft_sin_Complex(x);
	void	*cos = ft_cos_Complex(x);
	void	*retval = numeric_div(sin, cos);

	delete(sin);
	delete(cos);
	return (retval);
}

/* Return the tan of a number. */
void	*ft_tan(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_tan_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_tan_Complex(x);
			break ;
		default:
			fprintf(stderr, "tan function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the sinh of a Rational number */
struct s_Rational	*ft_sinh_Rational(struct s_Rational *_x)
{
	struct s_Rational	*x = copy(_x);
	struct s_Rational	*sum;
	void				*cosh;
	void				*sinh;
	void				*neg_x;
	void				*e_x;
	void				*e_neg_x;
	void				*two;
	mpz_t				quotient;

	mpz_init(quotient);
	/* scale until -1 < x < 1 */
	mpz_tdiv_q(quotient, x->numerator, x->denominator);
	if (mpz_cmp_ui(quotient, 0) != 0)
	{
		mpz_mul_ui(x->denominator, x->denominator, 2);

		/* sinh(2x) = 2 * sinh(x) * cosh(x) */
		cosh = ft_cosh_Rational(x);
		sinh = ft_sinh_Rational(x);
		sum = numeric_mul(cosh, sinh);
		mpz_mul_ui(sum->numerator, sum->numerator, 2);

		delete(cosh);
		delete(sinh);
		delete(x);
		mpz_clear(quotient);
		return (sum);
	}
	mpz_clear(quotient);

	neg_x = numeric_neg(x);
	e_x = ft_exp_Rational(x);
	e_neg_x = ft_exp_Rational(neg_x);
	sum = numeric_sub(e_x, e_neg_x);
	two = Rational_from_long(2, 1);

	numeric_idiv((void **)&sum, two);
	delete(x);
	delete(neg_x);
	delete(e_x);
	delete(e_neg_x);
	delete(two);
	return (sum);
}

/* Return the sinh of a Complex number
 * sinh(a + ib) = sinh(a) cosh(ib) + cosh(a) sinh(ib)
 *              = sinh(a) cos(b) + cosh(a) sinh(ib)    note: cos(z) = cosh(iz)
 *              = sinh(a) cos(b) + i cosh(a) sin(b)    note: i sin(z) = sinh(iz)
 */
struct s_Complex	*ft_sinh_Complex(struct s_Complex *x)
{
	void	*a = Complex_real(x);
	void	*b = Complex_imag(x);
	void	*sinh_a = ft_sinh_Rational(a);
	void	*cos_b = ft_cos_Rational(b);
	void	*cosh_a = ft_cosh_Rational(a);
	void	*sin_b = ft_sin_Rational(b);
	void	*real = numeric_mul(sinh_a, cos_b);
	void	*imag = numeric_mul(cosh_a, sin_b);

	delete(a);
	delete(b);
	delete(sinh_a);
	delete(cos_b);
	delete(cosh_a);
	delete(sin_b);
	return new(Complex, COMPLEX, real, imag);
}

/* Return the sinh of a number. */
void	*ft_sinh(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_sinh_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_sinh_Complex(x);
			break ;
		default:
			fprintf(stderr, "sinh function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the cosh of a Rational number */
struct s_Rational	*ft_cosh_Rational(struct s_Rational *_x)
{
	struct s_Rational	*x = copy(_x);
	void				*neg_x;
	void				*e_x;
	void				*e_neg_x;
	void				*retval;
	void				*one = Rational_from_long(1, 1);
	void				*two = Rational_from_long(2, 1);
	mpz_t				quotient;

	mpz_init(quotient);
	/* scale until -1 < x < 1 */
	mpz_tdiv_q(quotient, x->numerator, x->denominator);
	if (mpz_cmp_ui(quotient, 0) != 0)
	{
		mpz_mul_ui(x->denominator, x->denominator, 2);

		/* cosh(2x) = 2 (cosh(x))^2 - 1 */
		retval = ft_cosh_Rational(x);
		numeric_imul(&retval, retval);
		numeric_imul(&retval, two);
		numeric_isub(&retval, one);

		delete(x);
		delete(one);
		delete(two);
		mpz_clear(quotient);
		return (retval);
	}
	mpz_clear(quotient);

	/* cosh(x) = (e^x + e^(-x)) / 2 */
	neg_x = numeric_neg(x);
	e_x = ft_exp_Rational(x);
	e_neg_x = ft_exp_Rational(neg_x);
	retval = numeric_add(e_x, e_neg_x);
	numeric_idiv(&retval, two);

	delete(x);
	delete(neg_x);
	delete(e_x);
	delete(e_neg_x);
	delete(one);
	delete(two);
	return (retval);
}

/* Return the cosh of a Complex number
 * cosh(a + ib) = cosh(a) cosh(ib) + sinh(a) sinh(ib)
 *              = cosh(a) cos(b) + i sinh(a) sin(b)
 */
struct s_Complex	*ft_cosh_Complex(struct s_Complex *x)
{
	void	*a = Complex_real(x);
	void	*b = Complex_imag(x);
	void	*cosh_a = ft_cosh_Rational(a);
	void	*cos_b = ft_cos_Rational(b);
	void	*sinh_a = ft_sinh_Rational(a);
	void	*sin_b = ft_sin_Rational(b);
	void	*real = numeric_mul(cosh_a, cos_b);
	void	*imag = numeric_mul(sinh_a, sin_b);

	delete(a);
	delete(b);
	delete(cosh_a);
	delete(cos_b);
	delete(sinh_a);
	delete(sin_b);
	return new(Complex, COMPLEX, real, imag);
}

/* Return the cosh of a number. */
void	*ft_cosh(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_cosh_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_cosh_Complex(x);
			break ;
		default:
			fprintf(stderr, "cosh function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the tanh of a Rational number */
struct s_Rational	*ft_tanh_Rational(struct s_Rational *x)
{
	struct s_Rational	*sinh = ft_sinh_Rational(x);
	struct s_Rational	*cosh = ft_cosh_Rational(x);
	struct s_Rational	*retval = numeric_div(sinh, cosh);

	delete(sinh);
	delete(cosh);
	return (retval);
}

/* Return the tanh of a Complex number */
struct s_Complex	*ft_tanh_Complex(struct s_Complex *x)
{
	void	*sinh = ft_sinh_Complex(x);
	void	*cosh = ft_cosh_Complex(x);
	void	*retval = numeric_div(sinh, cosh);

	delete(sinh);
	delete(cosh);
	return (retval);
}

/* Return the tanh of a number. */
void	*ft_tanh(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_tanh_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_tanh_Complex(x);
			break ;
		default:
			fprintf(stderr, "tanh function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the square root of a Rational number */
void	*ft_sqrt_Rational(struct s_Rational *x)
{
	struct s_Rational	*half = Rational_from_double(0.5);
	struct s_Rational	*retval;

	if (x->numerator == 0)
		retval = copy(x);
	else
		retval = numeric_pow(x, half);
	delete(half);
	return (retval);
}

/* Return the square root of a Complex number */
struct s_Complex	*ft_sqrt_Complex(struct s_Complex *_x)
{
	struct s_Rational	*half = Rational_from_double(0.5);
	struct s_Rational	*real = Complex_real(_x);
	struct s_Rational	*imag = Complex_imag(_x);
	void				*x = NULL;
	void				*retval;

	if (imag->numerator == 0)	/* real number */
	{
		x = Complex_real(_x);
		retval = ft_sqrt_Rational(x);
		retval = numeric_ipromote(&retval, COMPLEX);
		delete(x);
	}
	else
		retval = numeric_pow(_x, half);
	delete(half);
	delete(real);
	delete(imag);
	return (retval);
}

/* Return the tan of a number. */
void	*ft_sqrt(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_sqrt_Rational(x);
			break ;
		case COMPLEX:
			retval = ft_sqrt_Complex(x);
			break ;
		default:
			fprintf(stderr, "sqrt function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the square root of a Rational number */
struct s_Rational	*ft_abs_Rational(struct s_Rational *x)
{
	if (mpz_cmp_ui(x->numerator, 0) < 0)
		return numeric_neg(x);
	else
		return copy(x);
}

/* Return the absolute value of a number. */
void	*ft_abs(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_abs_Rational(x);
			break ;
		case COMPLEX:
			retval = Complex_modulus(x);
			break ;
		case VECTOR:
			retval = Vector_magnitude(x);
			break ;
		case MATRIX:
			retval = Matrix_determinant(x);
			break ;
		default:
			fprintf(stderr, "sqrt function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Convert degree to radians */
struct s_Rational	*ft_radians_Rational(struct s_Rational *x)
{
	struct s_Rational	*pi = Rational_from_double(3.141592653589793);
	struct s_Rational	*n180 = Rational_from_double(180);
	void				*retval = numeric_mul(x, pi);

	retval = numeric_idiv(&retval, n180);
	delete(pi);
	delete(n180);
	return (retval);
}

/* Convert degree to radians */
void	*ft_radians(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_radians_Rational(x);
			break ;
		default:
			fprintf(stderr, "radians function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Convert radians to degrees */
struct s_Rational	*ft_degrees_Rational(struct s_Rational *x)
{
	struct s_Rational	*pi = Rational_from_double(3.141592653589793);
	struct s_Rational	*n180 = Rational_from_double(180);
	void				*retval = numeric_mul(x, n180);

	retval = numeric_idiv(&retval, pi);
	delete(pi);
	delete(n180);
	return (retval);
}

/* Convert radians to degrees */
void	*ft_degrees(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_degrees_Rational(x);
			break ;
		default:
			fprintf(stderr, "degrees function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* return the p = 1 norm of a matrix
 * reference: https://en.wikipedia.org/wiki/Matrix_norm
 */
struct s_Rational	*ft_norm_Matrix(struct s_Matrix *x)
{
	void	*sum;
	void	*temp;
	void	*retval;
	size_t	i;
	size_t	j;

	retval = Rational_from_double(0);
	for (j = 0; j < x->cols; ++j)
	{
		sum = Rational_from_double(0);
		for (i = 0; i < x->rows; ++i)
		{
			enum e_Tag	tag;

			temp = Matrix_at(x, i, j);
			tag = Token_get_tag(temp);
			if (tag == RATIONAL)
				temp = ft_abs_Rational(temp);
			else
				temp = Complex_modulus(temp);
			numeric_iadd(&sum, temp);
			delete(temp);
		}
		if (Rational_gt(sum, retval))
		{
			delete(retval);
			retval = sum;
		}
		else
			delete(sum);
	}
	return (retval);
}

/* return the p = 1 norm of a Vector
 * reference: https://en.wikipedia.org/wiki/Norm_(mathematics)
 */
struct s_Rational	*ft_norm_Vector(struct s_Vector *x)
{
	void	*sum;
	void	*temp;
	size_t	i;

	sum = Rational_from_double(0);
	for (i = 0; i < x->size; ++i)
	{
		enum e_Tag	tag;

		temp = Vector_at(x, i);
		tag = Token_get_tag(temp);
		if (tag == RATIONAL)
			temp = ft_abs_Rational(temp);
		else
			temp = Complex_modulus(temp);
		numeric_iadd(&sum, temp);
		delete(temp);
	}
	return (sum);
}

/* Return the norm of a number */
void	*ft_norm(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
		case COMPLEX:
			retval = ft_abs(params, env);
			break ;
		case VECTOR:
			retval = ft_norm_Vector(x);
			break ;
		case MATRIX:
			retval = ft_norm_Matrix(x);
			break ;
		default:
			fprintf(stderr, "norm function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the inverse of a number */
void	*ft_inv(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = Rational_invert(x);
			break ;
		case COMPLEX:
			retval = Complex_invert(x);
			break ;
		case MATRIX:
			retval = Matrix_invert(x);
			break ;
		default:
			fprintf(stderr, "inv function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}
