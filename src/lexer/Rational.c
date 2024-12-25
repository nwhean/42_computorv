#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

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

/* Rational constructor method. */
static void	*Rational_ctor(void *_self, va_list *app)
{
	struct s_Rational	*self;

	self = super_ctor(Rational, _self, app);
	mpz_init_set(self->numerator, va_arg(*app, mpz_ptr));
	mpz_init_set(self->denominator, va_arg(*app, mpz_ptr));
	return (Rational_canonicalise(self));
}

/* Rational destructor method. */
static void	*Rational_dtor(void *_self)
{
	struct s_Rational	*self = _self;

	mpz_clear(self->numerator);
	mpz_clear(self->denominator);
	return (super_dtor(Rational, self));
}

/* Construct a Rational from two long */
void	*Rational_from_long(long numerator, long denominator)
{
	mpz_t	num;
	mpz_t	den;
	void	*retval;

	mpz_init_set_si(num, numerator);
	mpz_init_set_si(den, denominator);
	retval = new(Rational, RATIONAL, num, den);
	mpz_clear(num);
	mpz_clear(den);
	return (retval);
}

/* Construct a Rational from double.
 * Relies on the rational number feature from GMP package.
 */
void	*Rational_from_double(double n)
{
	mpq_t	rational;
	mpz_t	num;
	mpz_t	den;
	void	*retval;

	mpq_init(rational);
	mpz_init(num);
	mpz_init(den);
	mpq_set_d(rational, n);
	mpq_get_num(num, rational);
	mpq_get_den(den, rational);
	retval = new(Rational, RATIONAL, num, den);
	mpq_clear(rational);
	mpz_clear(num);
	mpz_clear(den);
	return (retval);
}

/* convert Rational to a double. */
double	Rational_to_double(const struct s_Rational *self)
{
	return (mpz_get_d(self->numerator) / mpz_get_d(self->denominator));
}

/* Return a copy of the Rational. */
static struct s_Rational	*Rational_copy(const void *_self)
{
	const struct s_Rational	*self = _self;
	struct s_Rational		*retval;

	retval = super_copy(Rational, self);
	mpz_init_set(retval->numerator, self->numerator);
	mpz_init_set(retval->denominator, self->denominator);
	return (retval);
}

/* Return string representing the Rational. */
static char	*Rational_str(const void *_self)
{
	const struct s_Rational	*self = _self;
	int						len;
	char					*retval;

	if (mpz_cmp_ui(self->denominator, 1) == 0)
	{
		len = mpz_sizeinbase(self->numerator, 10);
		retval = malloc(len + 2);
		mpz_get_str(retval, 10, self->numerator);
	}
	else
	{
		len = DBL_MAX_10_EXP;
		retval = malloc(len + 3);
		sprintf(retval, "%f", Rational_to_double(self));
	}
	return (retval);
}

/* Return the addition of two Rationals. */
static void	*Rational_add_Rational(const void *_self, const void *_other)
{
	struct s_Rational	*lhs = (struct s_Rational *)_self;
	struct s_Rational	*rhs = (struct s_Rational *)_other;
	mpz_t				num;
	mpz_t				den;
	struct s_Rational	*retval;

	mpz_init(num);
	mpz_init(den);
	mpz_mul(num, lhs->numerator, rhs->denominator);		/* ad */
	mpz_addmul(num, rhs->numerator, lhs->denominator);	/* ad + bc */
	mpz_mul(den, lhs->denominator, rhs->denominator);	/* bd */

	/* put a hard limit on the size of the integers */
	while(mpz_sizeinbase(num, 2) > 512 || mpz_sizeinbase(den, 2) > 512)
	{
		mpz_div_ui(num, num, 2);
		mpz_div_ui(den, den, 2);
	}

	retval = new(Rational, RATIONAL, num, den);
	Rational_canonicalise(retval);

	mpz_clear(num);
	mpz_clear(den);
	return (retval);
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
	struct s_Rational	*lhs = (struct s_Rational *)_self;
	struct s_Rational	*rhs = (struct s_Rational *)_other;
	mpz_t				num;
	mpz_t				den;
	struct s_Rational	*retval;

	mpz_init(num);
	mpz_init(den);
	mpz_mul(num, lhs->numerator, rhs->denominator);		/* ad */
	mpz_submul(num, rhs->numerator, lhs->denominator);	/* ad - bc */
	mpz_mul(den, lhs->denominator, rhs->denominator);	/* bd */

	/* put a hard limit on the size of the integers */
	while(mpz_sizeinbase(num, 2) > 512 || mpz_sizeinbase(den, 2) > 512)
	{
		mpz_div_ui(num, num, 2);
		mpz_div_ui(den, den, 2);
	}

	retval = new(Rational, RATIONAL, num, den);
	Rational_canonicalise(retval);

	mpz_clear(num);
	mpz_clear(den);
	return (retval);
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
	struct s_Rational	*lhs = (struct s_Rational *)_self;
	struct s_Rational	*rhs = (struct s_Rational *)_other;
	mpz_t				num;
	mpz_t				den;
	struct s_Rational	*retval;

	mpz_init(num);
	mpz_init(den);
	mpz_mul(num, lhs->numerator, rhs->numerator);		/* ac */
	mpz_mul(den, lhs->denominator, rhs->denominator);	/* bd */

	/* put a hard limit on the size of the integers */
	while(mpz_sizeinbase(num, 2) > 512 || mpz_sizeinbase(den, 2) > 512)
	{
		mpz_div_ui(num, num, 2);
		mpz_div_ui(den, den, 2);
	}

	retval = new(Rational, RATIONAL, num, den);
	Rational_canonicalise(retval);

	mpz_clear(num);
	mpz_clear(den);
	return (retval);
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
	struct s_Rational	*lhs = (struct s_Rational *)_self;
	struct s_Rational	*rhs = (struct s_Rational *)_other;
	mpz_t				num;
	mpz_t				den;
	struct s_Rational	*retval;

	mpz_init(num);
	mpz_init(den);
	mpz_mul(num, lhs->numerator, rhs->denominator);		/* ad */
	mpz_mul(den, lhs->denominator, rhs->numerator);		/* bc */

	/* put a hard limit on the size of the integers */
	while(mpz_sizeinbase(num, 2) > 512 || mpz_sizeinbase(den, 2) > 512)
	{
		mpz_div_ui(num, num, 2);
		mpz_div_ui(den, den, 2);
	}

	retval = new(Rational, RATIONAL, num, den);
	Rational_canonicalise(retval);

	mpz_clear(num);
	mpz_clear(den);
	return (retval);
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
	struct s_Rational		*retval = Rational_from_long(0, 1);
	mpz_t					quotient;

	mpz_init(quotient);
	mpz_fdiv_q(quotient, self->numerator, self->denominator);
	mpz_set(retval->numerator, quotient);
	mpz_clear(quotient);
	return (retval);
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

	mpz_neg(retval->numerator, retval->numerator);
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
	mpz_t					a;
	mpz_t					c;
	mpz_t					d;
	mpz_t					quotient;
	mpz_t					remainder;
	void					*fraction = NULL;
	void					*cpy = NULL;
	void					*retval = NULL;
	void					*retval_frac = NULL;

	/* initialise values */
	mpz_init_set(a, self->numerator);
	mpz_init_set(c, other->numerator);
	mpz_init_set(d, other->denominator);

	/* raising negative number to non-integer gives a Complex solution */
	if (mpz_sgn(a) < 0 && mpz_cmp_ui(d, 1) != 0)
	{
		cpy = numeric_promote(self, COMPLEX);
		retval = numeric_pow(cpy, other);
		delete(cpy);
		mpz_clear(a);
		mpz_clear(c);
		mpz_clear(d);
		return (retval);
	}

	/* invert input if necessary */
	if (mpz_sgn(c) < 0)
	{
		cpy = Rational_invert(self);
		mpz_neg(c, c);
	}
	else
		cpy = copy(self);

	/* separate the power to integer and fractional portion */
	mpz_init(quotient);
	mpz_init(remainder);
	mpz_tdiv_qr(quotient, remainder, c, d);
	if (mpz_cmp_ui(remainder, 0) != 0)	/* if there's a fractional part */
	{
		fraction = new(Rational, RATIONAL, remainder, d);	/* b */
		retval = ft_ln_Rational(cpy);					/* ln a */
		fraction = numeric_imul(&fraction, retval);		/* b ln a */
		retval_frac = ft_exp_Rational(fraction);		/* e^(b ln a) */
		delete(retval);
	}

	/* exponentiation by squaring */
	/* reference: https://en.wikipedia.org/wiki/Exponentiation_by_squaring */
	retval = Rational_from_long(1, 1);
	while (mpz_cmp_ui(quotient, 1) > 0)
	{
		if (mpz_odd_p(quotient))	/* odd power */
		{
			retval = numeric_imul(&retval, cpy);
			mpz_sub_ui(quotient, quotient, 1);
		}
		cpy = numeric_imul(&cpy, cpy);
		mpz_div_ui(quotient, quotient, 2);
	}
	if (mpz_cmp_ui(quotient, 1) == 0)
		retval = numeric_imul(&retval, cpy);

	/* multiply with fractional power */
	if (retval_frac)
		retval = numeric_imul(&retval, retval_frac);

	mpz_clear(a);
	mpz_clear(c);
	mpz_clear(d);
	mpz_clear(quotient);
	mpz_clear(remainder);
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
	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	if (mpz_cmp(a->numerator, b->numerator))
		return (false);
	if (mpz_cmp(a->denominator, b->denominator))
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
	mpz_t	n1;
	mpz_t	n2;
	bool	retval;

	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	mpz_init_set(n1, a->numerator);
	mpz_init_set(n2, b->numerator);
	mpz_mul(n1, n1, b->denominator);
	mpz_mul(n2, n2, a->denominator);
	retval = mpz_cmp(n1, n2) < 0;
	mpz_clear(n1);
	mpz_clear(n2);
	return (retval);
}

/* Return true if a is greater than b. */
bool	Rational_gt(const struct s_Rational *a, const struct s_Rational *b)
{
	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	return (!Rational_le(a, b));
}

/* Return true if a is less than or equal to b, false otherwise. */
bool	Rational_le(const struct s_Rational *a, const struct s_Rational *b)
{
	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	return (Rational_eq(a, b) | Rational_lt(a, b));
}

/* Return true if a is greater than or equal to b, false otherwise. */
bool	Rational_ge(const struct s_Rational *a, const struct s_Rational *b)
{
	if (Token_get_tag(a) != Token_get_tag(b))
		return (false);
	return (!Rational_lt(a, b));
}

/* Promote one Numeric type to another */
static void	*Rational_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Rational	*self = _self;
	void					*retval;
	void					*vec_v;
	void					*vec_m;

	switch (tag)
	{
		case RATIONAL:
			return Rational_copy(self);
		case COMPLEX:
			return new(Complex, COMPLEX,
					Rational_copy(self),
					Rational_from_long(0, 1));
		case VECTOR:
		case MATRIX:
			/* create the vector */
			vec_v = new(Vec);
			Vec_push_back(vec_v, Rational_copy(self));
			if (tag == VECTOR)
			{
				retval = new(Vector, VECTOR, 1);
				Vector_update(retval, 0, Rational_copy(self));
				delete(vec_v);
				return (retval);
			}
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
				dtor, Rational_dtor,
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

	if (mpz_cmp_ui(self->numerator, 0) == 0)
		return (copy(self));
	return (new(Rational, RATIONAL, self->denominator, self->numerator));
}

/* Remove any factors that are common to the numerator and denominator
   and make the denominator positive. */
void	*Rational_canonicalise(void *_self)
{
	struct s_Rational	*self = _self;
	mpz_t				divisor;

	mpz_init(divisor);
	mpz_gcd(divisor, self->numerator, self->denominator);
	mpz_div(self->numerator, self->numerator, divisor);
	mpz_div(self->denominator, self->denominator, divisor);
	if (mpz_sgn(self->denominator) < 0)
	{
		mpz_neg(self->numerator, self->numerator);
		mpz_neg(self->denominator, self->denominator);
	}
	mpz_clear(divisor);
	return (self);
}