#ifndef RATIONAL_H
# define RATIONAL_H

# include <gmp.h>

# include "Numeric.h"

/* all members are public */
struct s_Rational	/* Rational: Numeric */
{
	const struct s_Numeric	_;
	void					*indeterminates;	/* Vec*/
	mpz_t					numerator;
	mpz_t					denominator;
};

extern const void	*Rational;	/* new(Rational, RATIONAL, long, long); */

void	initRational(void);
void	*Rational_from_long(long numerator, long denominator);
void	*Rational_from_double(double n);
double	Rational_to_double(const struct s_Rational *self);

/* relational */
bool	Rational_neq(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_lt(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_gt(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_le(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_ge(const struct s_Rational *a, const struct s_Rational *b);

/* operations */
bool	Rational_iszero(const void *self);
void	*Rational_invert(const void *self);
void	*Rational_canonicalise(void *self);

#endif
