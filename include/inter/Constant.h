#ifndef CONSTANT_H
# define CONSTANT_H

# include "Expr.h"

struct s_Constant	/* Constant: Expr */
{
	const struct s_Expr	_;
};

extern const void	*Constant;	/* new(Constant, Token, Tag); */

void	initConstant(void);

#endif
