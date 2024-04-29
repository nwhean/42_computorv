#ifndef RATIONAL_H
# define RATIONAL_H

# include "Numeric.h"

/* all members are public */
struct s_Rational	/* Rational: Numeric */
{
	const struct s_Numeric	_;
	long					numerator;
	long					denominator;
};

extern const void	*Rational;	/* new(Rational, RATIONAL, long, long); */

void	initRational(void);
void	*Rational_from_double(double n);
double	Rational_to_double(const struct s_Rational *self);

/* relational */
bool	Rational_neq(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_lt(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_gt(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_le(const struct s_Rational *a, const struct s_Rational *b);
bool	Rational_ge(const struct s_Rational *a, const struct s_Rational *b);

#endif
