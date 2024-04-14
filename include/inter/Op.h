#ifndef OP_H
# define OP_H

# include "Expr.h"

struct s_Op	/* Op: Expr */
{
	const struct s_Expr	_;
};

extern const void	*Op;	/* new(Op, Token, Tag); */

void	initOp(void);

#endif
