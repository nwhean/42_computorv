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

#endif
