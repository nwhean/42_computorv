#ifndef NUM_H
# define NUM_H

# include "Numeric.h"

// all members are public
struct s_Num		// Num: Numeric
{
	const struct s_Numeric	_;
	int						value;
};

extern const void	*Num;	// new(Num, NUM, value);

void	initNum(void);

#endif
