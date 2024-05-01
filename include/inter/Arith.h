#ifndef ARITH_H
# define ARITH_H

# include "Op.h"

/* Represents binary operators such as +, -, * and / */
struct s_Arith	/* Arith: Op */
{
	const struct s_Op	_;
	struct s_Expr		*expr1;
	struct s_Expr		*expr2;
};

extern const void	*Arith;	/* new(Arith, Token, ZERO, Expr, Expr); */

void	initArith(void);

#endif
