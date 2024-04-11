#ifndef REAL_H
# define REAL_H

# include "Token.h"

// all members are public
struct s_Real		// Real: Token
{
	const struct s_Token	_;
	double					value;
};

extern const void	*Real;	// new(Real, REAL, value);

void	initReal(void);

#endif
