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

/* other */
#include "mathematics.h"
#include "mathematics.r"
#include "utility.h"

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

/* Return the Least Common Multiple of two numbers. */
static long	lcm(long a, long b)
{
	return (labs(a * b) / gcd(a, b));
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

		a = floor(n / d);
		q2 = q0 + a * q1;				/* q2 = q0_prev + a * q1_prev */
		if (q2 > max_denominator || d == 0)
			break ;

		/* update p and q values for next cycle */
		swap_unsigned_long(&p0, &p1);	/* p0 = p1_prev */
		q0 = q1;						/* q0 = q1_prev */
		p1 += a * p0;					/* p1 = p0_prev + a * p1_prev */
		q1 = q2;						/* q1 = q0_prev + a * q1_prev = q2*/

		/* update n and d values for next cycle */
		swap_double(&n, &d);			/* n = d_prev */
		d -= a * n;						/* d = n_prev - a * d_prev */
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

/* Right shift the Rational to a simplier fraction. */
static void	*Rational_rshift(void *_self)
{
	struct s_Rational	*self = _self;
	long				a = self->numerator;
	long				b = self->denominator;

	while (b > 1e5)
	{
		if (a == -1)
		{
			a = 0;
			b = 1;
		}
		else
		{
			a >>= 1;
			b >>= 1;
		}
	}
	self->numerator = a;
	self->denominator = b;
	return (self);
}

/* Left shift the Rational to a simplier fraction. */
static void	*Rational_lshift(void *_self)
{
	struct s_Rational	*self = _self;
	long				a = self->numerator;
	long				b = self->denominator;

	while (labs(a) < 1e12 && b < 1e12)
	{
		{
			a <<= 1;
			b <<= 1;
		}
	}
	self->numerator = a;
	self->denominator = b;
	return (self);
}

/* Return the addition of two Rationals. */
static void	*Rational_add_Rational(const void *_self, const void *_other)
{
	struct s_Rational	*lhs = Rational_rshift(copy(_self));
	struct s_Rational	*rhs = Rational_rshift(copy(_other));
	long				a = lhs->numerator;
	long				b = lhs->denominator;
	long				c = rhs->numerator;
	long				d = rhs->denominator;
	long				g = gcd(b, d);

	delete(lhs);
	delete(rhs);
	return new(Rational, RATIONAL, a * (d / g) + c * (b / g), b * (d / g));
}

/* Return the addition of two Numerics. */
static void	*Rational_add(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
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
	struct s_Rational	*lhs = Rational_rshift(copy(_self));
	struct s_Rational	*rhs = Rational_rshift(copy(_other));
	long				a = lhs->numerator;
	long				b = lhs->denominator;
	long				c = rhs->numerator;
	long				d = rhs->denominator;
	long				g = gcd(b, d);

	delete(lhs);
	delete(rhs);
	return new(Rational, RATIONAL, a * (d / g) - c * (b / g), b * (d / g));
}

/* Return the subtraction of one Numeric from another. */
static void	*Rational_sub(const void *_self, const void *_other)
{
	void					*retval;

	switch (Token_get_tag(_other))
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
	struct s_Rational	*lhs = Rational_rshift(copy(_self));
	struct s_Rational	*rhs = Rational_rshift(copy(_other));
	long				a = lhs->numerator;
	long				b = lhs->denominator;
	long				c = rhs->numerator;
	long				d = rhs->denominator;
	long				f = gcd(a, d);
	long				g = gcd(b, c);

	delete(lhs);
	delete(rhs);
	return new(Rational, RATIONAL, (a / f) * (c / g), (b / g) * (d / f));
}

/* Return the multiplication of two Numerics. */
static void	*Rational_mul(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			return Rational_mul_Rational(_self, _other);
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
	struct s_Rational	*lhs = Rational_lshift(copy(_self));
	struct s_Rational	*rhs = Rational_rshift(copy(_other));
	long				a = lhs->numerator;
	long				b = lhs->denominator;
	long				c = rhs->numerator;
	long				d = rhs->denominator;
	long				f = gcd(a, c);
	long				g = gcd(b, d);

	delete(lhs);
	delete(rhs);
	return new(Rational, RATIONAL, (a / f) * (d / g), (b / g) * (c / f));
}

/* Return the division of one Numeric from another. */
static void	*Rational_div(const void *_self, const void *_other)
{
	void	*retval;

	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			return (Rational_div_Rational(_self, _other));
			break ;
		case COMPLEX:
			retval = numeric_promote(_self, COMPLEX);
			return (numeric_idiv(&retval, _other));
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
	switch (Token_get_tag(_other))
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
	long					c = other->numerator;
	long					d = other->denominator;
	long					integer;
	void					*fraction = NULL;
	void					*cpy = NULL;
	void					*retval = NULL;
	void					*retval_frac = NULL;

	/* raising negative number to non-integer gives a Complex solution */
	if (a < 0 && d != 1)
	{
		cpy = numeric_promote(self, COMPLEX);
		retval = numeric_pow(cpy, other);
		delete(cpy);
		return (retval);
	}

	/* invert input if necessary */
	if (c < 0)
	{
		cpy = Rational_invert(self);
		c = -c;
	}
	else
		cpy = copy(self);

	/* separate the power to integer and fractional portion */
	integer = c / d;
	if (c - integer * d)	/* if there's a fractional part */
	{
		fraction = new(Rational, RATIONAL, c - integer * d, d);	/* b */
		retval = ft_ln_Rational(cpy);					/* ln a */
		fraction = numeric_imul(&fraction, retval);		/* b ln a */
		retval_frac = ft_exp_Rational(fraction);		/* e^(b ln a) */
		delete(retval);
	}

	/* exponentiation by squaring */
	/* reference: https://en.wikipedia.org/wiki/Exponentiation_by_squaring */
	retval = new(Rational, RATIONAL, 1, 1);
	while (integer > 1)
	{
		if (integer % 2)	/* odd power */
		{
			retval = numeric_imul(&retval, cpy);
			--integer;
		}
		cpy = numeric_imul(&cpy, cpy);
		integer >>= 1;
	}
	if (integer == 1)
		retval = numeric_imul(&retval, cpy);

	/* multiply with fractional power */
	if (retval_frac)
		retval = numeric_imul(&retval, retval_frac);

	delete(cpy);
	delete(fraction);
	delete(retval_frac);
	return (retval);
}

/* Return the exponentiation of one Numeric to another. */
static void	*Rational_pow(const void *_self, const void *_other)
{
	void	*retval;

	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			return (Rational_pow_Rational(_self, _other));
		case COMPLEX:
			retval = numeric_promote(_self, COMPLEX);
			return (numeric_ipow(&retval, _other));
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
static bool	Rational_eq(const struct s_Rational *a, const struct s_Rational *b)
{
	long	n1 = a->numerator;
	long	d1 = a->denominator;
	long	n2 = b->numerator;
	long	d2 = b->denominator;

	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	if (n1 != n2)
		return (false);
	if (d1 != d2)
		return (false);
	return (true);
}

/* Return true if two Rational are not equal, false otherwise. */
bool	Rational_neq(const struct s_Rational *a, const struct s_Rational *b)
{
	return (!Rational_eq(a, b));
}

/* Return true if a is less than b. */
bool	Rational_lt(const struct s_Rational *a, const struct s_Rational *b)
{
	long	n1 = a->numerator;
	long	d1 = a->denominator;
	long	n2 = b->numerator;
	long	d2 = b->denominator;
	long	f;

	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	if (n1 == 0 && n2 == 0)
		return (false);
	if (n1 < 0 && n2 > 0)
		return (true);
	f = lcm(d1, d2);
	n1 *= f / d1;
	n2 *= f / d2;
	return (n1 < n2);
}

/* Return true if a is greater than b. */
bool	Rational_gt(const struct s_Rational *a, const struct s_Rational *b)
{
	long	n1 = a->numerator;
	long	d1 = a->denominator;
	long	n2 = b->numerator;
	long	d2 = b->denominator;
	long	f;

	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	if (n1 == 0 && n2 == 0)
		return (false);
	if (n1 > 0 && n2 < 0)
		return (true);
	f = lcm(d1, d2);
	n1 *= f / d1;
	n2 *= f / d2;
	return (n1 > n2);
}

/* Return true if a is less than or equal to b, false otherwise. */
bool	Rational_le(const struct s_Rational *a, const struct s_Rational *b)
{
	long	n1 = a->numerator;
	long	d1 = a->denominator;
	long	n2 = b->numerator;
	long	d2 = b->denominator;
	long	f;

	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	if (n1 == 0 && n2 == 0)
		return (true);
	if (n1 < 0 && n2 > 0)
		return (true);
	f = lcm(d1, d2);
	n1 *= f / d1;
	n2 *= f / d2;
	return (n1 <= n2);
}

/* Return true if a is greater than or equal to b, false otherwise. */
bool	Rational_ge(const struct s_Rational *a, const struct s_Rational *b)
{
	long	n1 = a->numerator;
	long	d1 = a->denominator;
	long	n2 = b->numerator;
	long	d2 = b->denominator;
	long	f;

	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	if (n1 == 0 && n2 == 0)
		return (true);
	if (n1 > 0 && n2 < 0)
		return (true);
	f = lcm(d1, d2);
	n1 *= f / d1;
	n2 *= f / d2;
	return (n1 >= n2);
}

/* Promote one Numeric type to another */
static void	*Rational_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Rational	*self = _self;
	void					*vec_v;
	void					*vec_m;

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
			if (tag == VECTOR)
				return (new(Vector, VECTOR, vec_v));

			/* create the matrix */
			vec_m = new(Vec);
			Vec_push_back(vec_m, vec_v);
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
				equal, Rational_eq,
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

/* Return the inverse of a Rational */
void	*Rational_invert(const void *_self)
{
	const struct s_Rational	*self = _self;
	long					a = self->numerator;
	long					b = self->denominator;

	if (a == 0)
		return (copy(self));
	else if (a < 0)
		return (new(Rational, RATIONAL, -b, a));
	else
		return (new(Rational, RATIONAL, b, a));
}
