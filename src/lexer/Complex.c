#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Complex.h"
#include "Rational.h"

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
static struct s_Complex	*Complex_from_polar(double modulus, double argument)
{
	double	a = modulus * cos(argument);
	double	b = modulus * sin(argument);

	return new(Complex, COMPLEX,
			Rational_from_double(a),
			Rational_from_double(b));
}

/* Return string representing the Complex. */
static char	*Complex_str(const void *_self)
{
	const struct s_Complex	*self = _self;
	const char *			str_real = str(self->real);
	const char *			str_imag = str(self->imag);
	int						len_real = strlen(str_real);
	int						len_imag = strlen(str_imag);
	int						len_new;
	char					*retval;

	len_new = len_real + (*str_imag != '-') + len_imag + 1;
	retval = calloc(sizeof(char), len_new + 1);
	memcpy(retval, str_real, len_real);
	if (*str_imag != '-')
		retval[len_real] = '+';
	memcpy(retval + len_real + (*str_imag != '-'), str_imag, len_imag);
	retval[len_real + (*str_imag != '-') + len_imag] = 'i';
	free((void *)str_real);
	free((void *)str_imag);
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
	const struct s_Token	*other = _other;
	struct s_Numeric		*temp = NULL;
	struct s_Numeric		*retval = NULL;

	switch (other->tag)
	{
		case RATIONAL:
			temp = numeric_promote(_other, COMPLEX);
			retval = Complex_add_Complex(_self, temp);
			break ;
		case COMPLEX:
			retval = Complex_add_Complex(_self, _other);
			break ;
	};
	delete(temp);
	return (retval);
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
	const struct s_Token	*other = _other;
	struct s_Numeric		*temp = NULL;
	struct s_Numeric		*retval = NULL;

	switch (other->tag)
	{
		case RATIONAL:
			temp = numeric_promote(_other, COMPLEX);
			retval = Complex_sub_Complex(_self, temp);
			break ;
		case COMPLEX:
			retval = Complex_sub_Complex(_self, _other);
			break ;
	};
	delete(temp);
	return (retval);
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
	const struct s_Token	*other = _other;
	struct s_Numeric		*temp = NULL;
	struct s_Numeric		*retval = NULL;

	switch (other->tag)
	{
		case RATIONAL:
			temp = numeric_promote(_other, COMPLEX);
			retval = Complex_mul_Complex(_self, temp);
			break ;
		case COMPLEX:
			retval = Complex_mul_Complex(_self, _other);
			break ;
	};
	delete(temp);
	return (retval);
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
	struct s_Complex		*numerator = numeric_mul(self, conj);
	struct s_Rational		*c = other->real;
	struct s_Rational		*d = other->imag;
	struct s_Rational		*c2 = numeric_mul(c, c);
	struct s_Rational		*d2 = numeric_mul(d, d);
	struct s_Rational		*denominator = numeric_add(c2, d2);
	struct s_Complex		*retval;

	retval = Complex_div_Rational(numerator, denominator);
	delete(conj);
	delete(numerator);
	delete(c2);
	delete(d2);
	delete(denominator);
	return (retval);
}

/* Return the division of one Complex to another Numeric. */
static void	*Complex_div(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
			return Complex_div_Rational(self, other);
		case COMPLEX:
			return Complex_div_Complex(self, other);
	};
	return (NULL);
}

static void	*Complex_mod(const void *_self, const void *_other)
{
	(void)_self;
	(void)_other;
	fprintf(stderr, "%s\n", "%% operator is not defined for Complex.");
	return (NULL);
}

/* Return a copy of the Complex with its value negated. */
static struct s_Complex	*Complex_neg(const void *_self)
{
	const struct s_Complex	*self = _self;
	struct s_Rational		*real = Complex_real(self);
	struct s_Rational		*imag = Complex_imag(self);

	real->numerator *= -1;
	imag->numerator *= -1;
	return (new(Complex, COMPLEX, real, imag));
}

/* Return the exponentiation of a Complex to a Rational power. */
static void	*Complex_pow_Rational(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Rational	*other = _other;
	struct s_Rational		*modulus = Complex_modulus(self);
	struct s_Rational		*argument = Complex_argument(self);
	double					r = Rational_to_double(modulus);
	double					theta = Rational_to_double(argument);
	double					n = Rational_to_double(other);

	r = pow(r, n);
	theta *= n;
	delete(modulus);
	delete(argument);
	return (Complex_from_polar(r, theta));
}

/* Return the exponentiation of a Complex to a Complex power, using the
 * principal branch of the logarithm for complex exponentiation.*/
static void	*Complex_pow_Complex(const void *_self, const void *_other)
{
	const struct s_Complex	*self = _self;
	const struct s_Complex	*other = _other;
	struct s_Rational	*modulus = Complex_modulus(self);
	struct s_Rational	*log_modulus = Rational_from_double(
										log(Rational_to_double(modulus)));
	struct s_Complex		*a = new(Complex, COMPLEX,
									log_modulus,
									Complex_argument(self));
	struct s_Complex		*mul = numeric_mul(a, other);
	double					r = exp(Rational_to_double(mul->real));
	double					theta = Rational_to_double(mul->imag);

	delete(modulus);
	delete(a);
	delete(mul);
	return (Complex_from_polar(r, theta));
}

/* Return the exponentiation of one Numeric to another. */
static void	*Complex_pow(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
			return (Complex_pow_Rational(_self, _other));
		case COMPLEX:
			return (Complex_pow_Complex(_self, _other));
	};
	return (NULL);
}

void	initComplex(void)
{
	if (!Complex)
	{
		initNumeric();
		Complex = new(NumericClass, "Complex",
				Numeric, sizeof(struct s_Complex),
				ctor, Complex_ctor,
				copy, Complex_copy,
				dtor, Complex_dtor,
				str, Complex_str,
				numeric_add, Complex_add,
				numeric_sub, Complex_sub,
				numeric_mul, Complex_mul,
				numeric_div, Complex_div,
				numeric_mod, Complex_mod,
				numeric_pos, Complex_copy,
				numeric_neg, Complex_neg,
				numeric_pow, Complex_pow,
				0);
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
	struct s_Rational	*real = Complex_real(self);
	struct s_Rational	*imag = Complex_imag(self);

	imag->numerator *= -1;
	return new(Complex, COMPLEX, real, imag);
}
