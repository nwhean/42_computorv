#ifndef COMPLEX_H
# define COMPLEX_H

# include "Numeric.h"
# include "Rational.h"

/* all members are public */
struct s_Complex	/* Complex: Numeric */
{
	const struct s_Numeric	_;
	struct s_Rational		*real;
	struct s_Rational		*imag;
};

extern const void	*Complex;	/* new(Complex, COMPLEX, Rational, Rational); */

void	initComplex(void);
struct s_Rational	*Complex_real(const struct s_Complex *self);
struct s_Rational	*Complex_imag(const struct s_Complex *self);
struct s_Rational	*Complex_modulus(const struct s_Complex *self);
struct s_Rational	*Complex_argument(const struct s_Complex *self);
struct s_Complex	*Complex_conjugate(const struct s_Complex *self);
struct s_Complex	*Complex_invert(const void *self);


#endif
