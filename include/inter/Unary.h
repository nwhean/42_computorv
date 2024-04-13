#ifndef UNARY_H
# define UNARY_H

# include "Op.h"

// Represents binary operators such as +, -, * and /
struct s_Unary			// Unary: Op
{
	const struct s_Op	_;
	struct s_Expr		*expr;
};

extern const void	*Unary;			// new(Unary, Token, NULL, expr);

void	initUnary(void);

#endif
