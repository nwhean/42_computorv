#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Vec.h"

/* lexer */
#include "Rational.h"
#include "Complex.h"
#include "Vector.h"
#include "Matrix.h"

const void	*Rational;

/* Return the Greatest Common Divisor of two numbers. */
static long	gcd(long a, long b)
{
	a = labs(a);
	b = labs(b);
	while (a && b)
	{
		if (a > b)
			a %= b;
		else
			b %= a;
	}
	if (!a)
		return (b);
	return (a);
}

/* Rational constructor method. */
static void	*Rational_ctor(void *_self, va_list *app)
{
	struct s_Rational	*self;
	long				numerator;
	long				denominator;
	long				divisor;

	self = super_ctor(Rational, _self, app);
	numerator = va_arg(*app, long);
	denominator = va_arg(*app, long);
	divisor = gcd(numerator, denominator);
	numerator /= divisor;
	denominator /= divisor;
	if (denominator < 0)
	{
		numerator *= -1;
		denominator *= -1;
	}
	self->numerator = numerator;
	self->denominator = denominator;
	return (self);
}

/* Construct a Rational from double.
 * Adapted from Python source code:
 * https://github.com/python/cpython/blob/a0f82dd6ccddc5fd3266df8ba55496ab573aacf2/Lib/fractions.py#L340
 */
void	*Rational_from_double(double n)
{
	const unsigned long	max_denominator = 1.0e+9;	/* precision */
	unsigned long		p0 = 0;
	unsigned long		q0 = 1;
	unsigned long		p1 = 1;
	unsigned long		q1 = 0;
	double				d = 1;
	long				sign = n < 0 ? -1 : 1;

	if (n != n)		/* if n == nan, this evaluates to true */
		return (NULL);

	n = fabs(n);	/* simplify the algorithm by only using positive numbers */
	while (1)
	{
		unsigned long	q2;
		unsigned long	a;
		unsigned long	temp_l;
		double	temp_d;

		a = floor(n / d);
		q2 = q0 + a * q1;
		if (q2 > max_denominator || d == 0)
			break ;

		/* update p and q values for next cycle */
		temp_l = p0;
		p0 = p1;
		q0 = q1;
		p1 = temp_l + a * p1;	/* p1 = p0 + a * p1 */
		q1 = q2;				/* q1 = q0 + a * q1 */

		/* update n and d values for next cycle */
		temp_d = n;
		n = d;
		d = temp_d - a * d;
	}
	return new(Rational, RATIONAL, sign * p1, q1);
}

/* convert Rational to a double. */
double	Rational_to_double(const struct s_Rational *self)
{
	return ((double)(self->numerator) / self->denominator);
}

/* Return a copy of the Rational. */
static struct s_Rational	*Rational_copy(const void *_self)
{
	const struct s_Rational	*self = _self;
	struct s_Rational		*retval;

	retval = super_copy(Rational, self);
	retval->numerator = self->numerator;
	retval->denominator = self->denominator;
	return (retval);
}

/* Return string representing the Rational. */
static char	*Rational_str(const void *_self)
{
	const struct s_Rational	*self = _self;
	int						len;
	char					*retval;

	if (self->denominator == 1)
	{
		len = ceil(log10(LONG_MAX));
		retval = malloc(len + 2);
		sprintf(retval, "%ld", self->numerator);
	}
	else
	{
		len = DBL_MAX_10_EXP;
		retval = malloc(len + 3);
		sprintf(retval, "%f",
				(double)self->numerator / self->denominator);
	}
	return (retval);
}

/* Return the addition of two Rationals. */
static void	*Rational_add_Rational(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Rational	*other = _other;
	long					a = self->numerator;
	long					b = self->denominator;
	long					c = other->numerator;
	long					d = other->denominator;
	long					g = gcd(b, d);

	return new(Rational, RATIONAL, a * (d / g) + c * (b / g), b * (d / g));
}

/* Return the addition of two Numerics. */
static void	*Rational_add(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
			return (Rational_add_Rational(_self, _other));
		case COMPLEX:
		case VECTOR:
		case MATRIX:
			return (numeric_add(_other, _self));
		default:
			fprintf(stderr, "%s\n", "Rational_add: unexpected input type.");
			return (NULL);
	};
}

/* Return the subtraction of one Rational from Rational. */
static void	*Rational_sub_Rational(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Rational	*other = _other;
	long					a = self->numerator;
	long					b = self->denominator;
	long					c = other->numerator;
	long					d = other->denominator;
	long					g = gcd(b, d);

	return new(Rational, RATIONAL, a * (d / g) - c * (b / g), b * (d / g));
}

/* Return the subtraction of one Numeric from another. */
static void	*Rational_sub(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;
	void					*retval;

	switch (other->tag)
	{
		case RATIONAL:
			return (Rational_sub_Rational(_self, _other));
		case COMPLEX:
		case VECTOR:
		case MATRIX:
			retval = numeric_sub(_other, _self);
			return (numeric_ineg(&retval));
		default:
			fprintf(stderr, "%s\n", "Rational_sub: unexpected input type.");
			return (NULL);
	};
}

/* Return the multiplication of two Rationals. */
static void	*Rational_mul_Rational(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Rational	*other = _other;
	long					a = self->numerator;
	long					b = self->denominator;
	long					c = other->numerator;
	long					d = other->denominator;
	long					f = gcd(a, d);
	long					g = gcd(b, c);

	return new(Rational, RATIONAL, (a / f) * (c / g), (b / g) * (d / f));
}

/* Return the multiplication of two Numerics. */
static void	*Rational_mul(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
			return Rational_mul_Rational(self, other);
		case COMPLEX:
		case VECTOR:
		case MATRIX:
			return numeric_mul(_other, _self);
		default:
			fprintf(stderr, "%s\n", "Rational_mul: unexpected input type.");
			return (NULL);
	};
}

/* Return the division of one Rational from another. */
static void	*Rational_div_Rational(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Rational	*other = _other;
	long					a = self->numerator;
	long					b = self->denominator;
	long					c = other->numerator;
	long					d = other->denominator;
	long					f = gcd(a, c);
	long					g = gcd(b, d);

	return new(Rational, RATIONAL, (a / f) * (d / g), (b / g) * (c / f));
}

/* Return the division of one Numeric from another. */
static void	*Rational_div(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Token	*other = _other;
	void					*retval;

	switch (other->tag)
	{
		case RATIONAL:
			return (Rational_div_Rational(self, other));
			break ;
		case COMPLEX:
			retval = numeric_promote(self, COMPLEX);
			return (numeric_idiv(&retval, other));
		case VECTOR:
			fprintf(stderr, "%s\n", "Rational_div: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Rational_div: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Rational_div: unexpected input type.");
			return (NULL);
	};
}

/* Rounds a Rational down to an integer value. */
static void	*Rational_floor(const void *_self)
{
	const struct s_Rational	*self = _self;
	long					num;

	num = (long)floor((double)self->numerator / (double)self->denominator);
	return new(Rational, RATIONAL, num, 1);
}

/* Return remainder from the division of one Rational from another. */
static void	*Rational_mod_Rational(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Rational	*other = _other;
	struct s_Rational 		*multiplier = Rational_div(_self, _other);
	struct s_Rational		*integer = Rational_floor(multiplier);
	struct s_Rational		*whole =  Rational_mul(other, integer);
	struct s_Rational		*retval = Rational_sub(self, whole);

	delete(multiplier);
	delete(integer);
	delete(whole);
	return (retval);
}

/* Return remainder from the division of one Numeric from another. */
static void	*Rational_mod(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
			return (Rational_mod_Rational(_self, _other));
		case COMPLEX:
			fprintf(stderr, "%s\n", "Rational_mod: incompatible with Complex");
			return (NULL);
		case VECTOR:
			fprintf(stderr, "%s\n", "Rational_mod: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Rational_mod: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Rational_mod: unexpected input type.");
			return (NULL);
	};
}

/* Return a copy of the Rational with its value negated. */
static struct s_Rational	*Rational_neg(const void *_self)
{
	const struct s_Rational	*self = _self;
	struct s_Rational		*retval = copy(self);

	retval->numerator *= -1;
	return (retval);
}

/* Return the exponentiation of one Rational to another.
 *
 * Since the function relies on 'pow', it does not support negative number
 * raised to a non-integer number, as that will return a complex number.
 */
static void	*Rational_pow_Rational(const void *_self, const void *_other)
{
	const struct s_Rational	*self = _self;
	const struct s_Rational	*other = _other;
	long					a = self->numerator;
	long					b = self->denominator;
	long					c = other->numerator;
	long					d = other->denominator;
	double					value = pow((double)a / b, (double)c / d);
	struct s_Numeric		*promoted;
	struct s_Numeric		*retval;

	if (a < 0 && d != 1)
	{
		if (!Complex)
		{
			fprintf(stderr, "%s\n",
					"(-Rational)^(non-integer) is not supported");
			return (NULL);
		}
		promoted = numeric_promote(self, COMPLEX);
		retval = numeric_pow(promoted, other);
		delete(promoted);
		return (retval);
	}
	return Rational_from_double(value);
}

/* Return the exponentiation of one Numeric to another. */
static void	*Rational_pow(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;
	void					*retval;

	switch (other->tag)
	{
		case RATIONAL:
			return (Rational_pow_Rational(_self, _other));
		case COMPLEX:
			retval = numeric_promote(_self, COMPLEX);
			return (numeric_ipow(&retval, other));
		case VECTOR:
			fprintf(stderr, "%s\n", "Rational_pow: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Rational_pow: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Rational_pow: unexpected input type.");
			return (NULL);
	};
}

/* Return true if two Rationals are the same, false otherwise. */
static bool	Rational_equal(
		const struct s_Rational *_self, const struct s_Rational *_other)
{
	const struct s_Token	*self = (const struct s_Token *)_self;
	const struct s_Token	*other = (const struct s_Token *)_other;

	if (self->tag != other->tag)
		return (false);
	if (_self->numerator != _other->numerator)
		return (false);
	if (_self->denominator != _other->denominator)
		return (false);
	return (true);
}

/* Promote one Numeric type to another */
static void	*Rational_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Rational	*self = _self;
	void					*vec_v;
	void					*vec_m;
	struct s_Vector			*v;

	switch (tag)
	{
		case RATIONAL:
			return Rational_copy(self);
		case COMPLEX:
			return new(Complex, COMPLEX,
					Rational_copy(self),
					new(Rational, RATIONAL, 0, 1));
		case VECTOR:
		case MATRIX:
			/* create the vector */
			vec_v = new(Vec);
			Vec_push_back(vec_v, Rational_copy(self));
			v = new(Vector, VECTOR, vec_v);
			if (tag == VECTOR)
				return (v);

			/* create the matrix */
			vec_m = new(Vec);
			Vec_push_back(vec_m, v);
			return new(Matrix, MATRIX, vec_m);
		default:
			fprintf(stderr, "%s\n",
					"Rational_promotion: unexpected input type.");
			return (NULL);
	};
}

void	initRational(void)
{
	if (!Rational)
	{
		initNumeric();
		Rational = new(NumericClass, "Rational",
				Numeric, sizeof(struct s_Rational),
				ctor, Rational_ctor,
				copy, Rational_copy,
				str, Rational_str,
				equal, Rational_equal,
				numeric_add, Rational_add,
				numeric_sub, Rational_sub,
				numeric_mul, Rational_mul,
				numeric_div, Rational_div,
				numeric_mod, Rational_mod,
				numeric_neg, Rational_neg,
				numeric_pow, Rational_pow,
				numeric_promote, Rational_promote,
				0);
		initVec();
		initComplex();
		initVector();
		initMatrix();
	}
}
