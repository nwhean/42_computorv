#include <math.h>
#include <stdlib.h>

/* container */
#include "Str.h"
#include "Vec.h"

/* lexer */
#include "Rational.h"
#include "Complex.h"
#include "Vector.h"
#include "Matrix.h"

/* other */
#include "mathematics.h"
#include "mathematics.r"

const void	*Complex;

/* Complex constructor method. */
static void	*Complex_ctor(void *_self, va_list *app)
{
	struct s_Complex	*self;

	self = super_ctor(Complex, _self, app);
	self->real = va_arg(*app, struct s_Rational *);
	self->imag = va_arg(*app, struct s_Rational *);
	return (self);
}

/* Complex destructor method. */
static void	*Complex_dtor(void *_self)
{
	struct s_Complex	*self = _self;

	delete(self->real);
	delete(self->imag);
	return(super_dtor(Complex, _self));
}

/* Return a copy of the Complex. */
static struct s_Complex	*Complex_copy(const void *_self)
{
	const struct s_Complex	*self = _self;
	struct s_Complex		*retval;

	retval = super_copy(Complex, self);
	retval->real = copy(self->real);
	retval->imag = copy(self->imag);
	return (retval);
}

/* Return a Complex number given its polar form */
static struct s_Complex	*Complex_from_polar(double _modulus, double _argument)
{
	void	*modulus = Rational_from_double(_modulus);
	void	*argument = Rational_from_double(_argument);
	void	*cos = ft_cos_Rational(argument);
	void	*sin = ft_sin_Rational(argument);
	void	*a = numeric_mul(modulus, cos);
	void	*b = numeric_mul(modulus, sin);

	delete(modulus);
	delete(argument);
	delete(cos);
	delete(sin);
	return new(Complex, COMPLEX, a, b);
}

/* Return string representing the Complex. */
static char	*Complex_str(const void *_self)
{
	const struct s_Complex	*self = _self;
	const char *			str_real = str(self->real);
	const char *			str_imag = str(self->imag);
	void					*s = new(Str, str_real);
	char					*retval;

	if (*str_imag != '-')
		Str_push_back(s, '+');
	Str_append(s, str_imag);
	Str_push_back(s, 'i');
	retval = str(s);
	free((void *)str_real);
	free((void *)str_imag);
	delete(s);
	return (retval);
}

/* Return the addition of two Complexs. */
static void	*Complex_add_Complex(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Complex	*other = _other;
	const struct s_Rational	*a = self->real;
	const struct s_Rational	*b = self->imag;
	const struct s_Rational	*c = other->real;
	const struct s_Rational	*d = other->imag;

	return new(Complex, COMPLEX, numeric_add(a, c), numeric_add(b, d));
}

/* Return the addition of two Numerics. */
static void	*Complex_add(const void *_self, const void *_other)
{
	void					*retval = NULL;

	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			retval = numeric_promote(_other, COMPLEX);
			return (numeric_iadd(&retval, _self));
		case COMPLEX:
			return (Complex_add_Complex(_self, _other));
		case VECTOR:
		case MATRIX:
			return (numeric_add(_other, _self));
		default:
			fprintf(stderr, "%s\n", "Rational_add: unexpected input type.");
			return (NULL);
	};
}

/* Return the subtraction of one Complex from Complex. */
static void	*Complex_sub_Complex(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Complex	*other = _other;
	const struct s_Rational	*a = self->real;
	const struct s_Rational	*b = self->imag;
	const struct s_Rational	*c = other->real;
	const struct s_Rational	*d = other->imag;

	return new(Complex, COMPLEX, numeric_sub(a, c), numeric_sub(b, d));
}

/* Return the subtraction of one Numeric from another. */
static void	*Complex_sub(const void *_self, const void *_other)
{
	void					*retval;

	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			retval = numeric_promote(_other, COMPLEX);
			numeric_ineg(&retval);
			numeric_isub(&retval, _self);
			return (numeric_ineg(&retval));
		case COMPLEX:
			return (Complex_sub_Complex(_self, _other));
		case VECTOR:
		case MATRIX:
			retval = numeric_sub(_other, _self);
			return (numeric_ineg(&retval));
		default:
			fprintf(stderr, "%s\n", "Rational_sub: unexpected input type.");
			return (NULL);
	};
}

/* Return the multiplication of two Complexs. */
static void	*Complex_mul_Complex(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Complex	*other = _other;
	struct s_Rational	*a = self->real;
	struct s_Rational	*b = self->imag;
	struct s_Rational	*c = other->real;
	struct s_Rational	*d = other->imag;
	struct s_Rational	*ac = numeric_mul(a, c);
	struct s_Rational	*bd = numeric_mul(b, d);
	struct s_Rational	*ad = numeric_mul(a, d);
	struct s_Rational	*bc = numeric_mul(b, c);
	struct s_Complex	*retval;

	retval = new(Complex, COMPLEX, numeric_sub(ac, bd), numeric_add(ad, bc));
	delete(ac);
	delete(bd);
	delete(ad);
	delete(bc);
	return (retval);
}

/* Return the multiplication of two Numerics. */
static void	*Complex_mul(const void *_self, const void *_other)
{
	void					*retval;

	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			retval = numeric_promote(_other, COMPLEX);
			return (numeric_imul(&retval, _self));
		case COMPLEX:
			return (Complex_mul_Complex(_self, _other));
		case VECTOR:
		case MATRIX:
			return numeric_mul(_other, _self);
		default:
			fprintf(stderr, "%s\n", "Complex_mul: unexpected input type.");
			return (NULL);
	};
}

/* Return the division of one Complex and a Rational. */
static void	*Complex_div_Rational(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Rational	*other = _other;
	struct s_Rational		*a = self->real;
	struct s_Rational		*b = self->imag;
	struct s_Complex		*retval;

	retval = new(Complex, COMPLEX,
				numeric_div(a, other),
				numeric_div(b, other));
	return (retval);
}

/* Return the division of one Complex from another. */
static void	*Complex_div_Complex(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Complex	*other = _other;
	struct s_Complex		*conj = Complex_conjugate(_other);
	struct s_Complex		*num = numeric_mul(self, conj);
	struct s_Complex		*den = numeric_mul(other, conj);
	struct s_Complex		*retval = numeric_div(num, den->real);

	delete(conj);
	delete(num);
	delete(den);
	return (retval);
}

/* Return the division of one Complex to another Numeric. */
static void	*Complex_div(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			return Complex_div_Rational(_self, _other);
		case COMPLEX:
			return Complex_div_Complex(_self, _other);
		case VECTOR:
			fprintf(stderr, "%s\n", "Complex_div: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Complex_div: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Complex_div: unexpected input type.");
			return (NULL);
	};
}

/* Return a copy of the Complex with its value negated. */
static struct s_Complex	*Complex_neg(const void *_self)
{
	const struct s_Complex	*self = _self;
	struct s_Complex		*retval = copy(self);

	numeric_ineg((void **)&retval->real);
	numeric_ineg((void **)&retval->imag);
	return (retval);
}

/* Return the exponentiation of a Complex to a Rational power.
 * e^(ix) = cos(x) + i sin(x)
 * (e^(ix))^y = e^(ixy) = cos(xy) + i sin(xy)
 */
static void	*Complex_pow_Rational(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Rational	*other = _other;
	void					*modulus = Complex_modulus(self);
	void					*argument = Complex_argument(self);
	double 					r;
	double					theta;

	modulus = numeric_ipow(&modulus, other);
	argument = numeric_imul(&argument, other);
	r = Rational_to_double(modulus);
	theta = Rational_to_double(argument);
	delete(modulus);
	delete(argument);
	return (Complex_from_polar(r, theta));
}

/* Return the exponentiation of a Complex to a Complex power, using the
 * principal branch of the logarithm for complex exponentiation.
 *
 * z1 ^ z2 = (a + ib)^(c + id)
 *         = (re^ix)^(c + id)		r = modulus(z1), x = arg(z1)
 *         = (e^(ln(r)+ix))^(c + id)
 *         = e^(ln(r)*(c + id) + ix*(c + id))
 */
static void	*Complex_pow_Complex(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Complex	*other = _other;
	struct s_Rational		*modulus = Complex_modulus(self);		/* r */
	struct s_Rational		*log_modulus = ft_ln_Rational(modulus); /* ln(r) */
	struct s_Complex		*a = new(Complex, COMPLEX,
									log_modulus,
									Complex_argument(self));	/* ln(r) + ix */
	struct s_Complex		*mul = numeric_mul(a, other);
	struct s_Rational		*modulus_new = ft_exp_Rational(mul->real);
	double					r = Rational_to_double(modulus_new);
	double					theta = Rational_to_double(mul->imag);

	delete(modulus);
	delete(modulus_new);
	delete(a);
	delete(mul);
	return (Complex_from_polar(r, theta));
}

/* Return the exponentiation of one Numeric to another. */
static void	*Complex_pow(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			return (Complex_pow_Rational(_self, _other));
		case COMPLEX:
			return (Complex_pow_Complex(_self, _other));
		case VECTOR:
			fprintf(stderr, "%s\n", "Complex_pow: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Complex_pow: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Complex_pow: unexpected input type.");
			return (NULL);
	};
}

/* Return true if two Complexes are the same, false otherwise. */
static bool	Complex_equal(
		const struct s_Complex *_self, const struct s_Complex *_other)
{
	if (Token_get_tag(_self) != Token_get_tag(_other))
		return (false);
	if (!numeric_equal(_self->real, _other->real))
		return (false);
	if (!numeric_equal(_self->imag, _other->imag))
		return (false);
	return (true);
}

/* Promote one Numeric type to another */
static void	*Complex_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Complex	*self = _self;
	void					*vec_v;
	void					*vec_m;

	switch (tag)
	{
		case RATIONAL:
			fprintf(stderr, "%s\n",
					"Complex_promotion: cannot demote to Rational.");
			return (NULL);
		case COMPLEX:
			return Complex_copy(self);
		case VECTOR:
		case MATRIX:
			/* create the vector */
			vec_v = new(Vec);
			Vec_push_back(vec_v, Complex_copy(self));
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

void	initComplex(void)
{
	initNumeric();
	if (!Complex)
	{
		Complex = new(NumericClass, "Complex",
				Numeric, sizeof(struct s_Complex),
				ctor, Complex_ctor,
				copy, Complex_copy,
				dtor, Complex_dtor,
				str, Complex_str,
				equal, Complex_equal,
				numeric_add, Complex_add,
				numeric_sub, Complex_sub,
				numeric_mul, Complex_mul,
				numeric_div, Complex_div,
				numeric_neg, Complex_neg,
				numeric_pow, Complex_pow,
				numeric_promote, Complex_promote,
				0);
		initStr();
		initVec();
		initRational();
		initVector();
		initMatrix();
	}
}

/* Return the real part of a Complex number. */
struct s_Rational	*Complex_real(const struct s_Complex *self)
{
	return copy(self->real);
}

/* Return the imaginary part of a complex number. */
struct s_Rational	*Complex_imag(const struct s_Complex *self)
{
	return copy(self->imag);
}

/* Return the modulus of a complex number. */
struct s_Rational	*Complex_modulus(const struct s_Complex *self)
{
	struct s_Rational	*a = self->real;
	struct s_Rational	*b = self->imag;
	struct s_Rational	*a2 = numeric_mul(a, a);
	struct s_Rational	*b2 = numeric_mul(b, b);
	struct s_Rational	*sum = numeric_add(a2, b2);
	struct s_Rational	*retval;

	retval = Rational_from_double(
				sqrt((double)sum->numerator / sum->denominator));
	delete(a2);
	delete(b2);
	delete(sum);
	return (retval);
}

/* Return the argument of a complex number. */
struct s_Rational	*Complex_argument(const struct s_Complex *self)
{
	struct s_Rational	*a = self->real;
	struct s_Rational	*b = self->imag;
	double				x = Rational_to_double(a);
	double				y = Rational_to_double(b);

	return (Rational_from_double(atan2(y, x)));
}

/* Return the conjugate of a Complex number. */
struct s_Complex	*Complex_conjugate(const struct s_Complex *self)
{
	void					*real;
	void					*imag;

	if (Token_get_tag(self) == RATIONAL)
		return (copy(self));
	real = copy(self->real);
	imag = copy(self->imag);
	numeric_ineg(&imag);
	return (new(Complex, COMPLEX, real, imag));
}
