#ifndef NUM_H
# define NUM_H

# include "Token.h"

// all members are public
struct s_Num		// Num: Token
{
	const struct s_Token	_;
	int						value;
};

extern const void	*Num;	// new(Point, ...);

void	initNum(void);

#endif
