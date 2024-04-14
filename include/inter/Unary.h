#ifndef UNARY_H
# define UNARY_H

# include "Op.h"

/* Represents unary operators such as + and - */
struct s_Unary	/* Unary: Op */
{
	const struct s_Op	_;
	struct s_Expr		*expr;
};

extern const void	*Unary;	/* new(Unary, Token, 0, Expr); */

void	initUnary(void);

#endif
