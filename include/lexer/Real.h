#ifndef REAL_H
# define REAL_H

# include "Numeric.h"

// all members are public
struct s_Real		// Real: Numeric
{
	const struct s_Numeric	_;
	double					value;
};

extern const void	*Real;	// new(Real, REAL, value);

void	initReal(void);

#endif
