#ifndef VECEXPR_H
# define VECEXPR_H

# include "Expr.h"

struct s_VecExpr	/* VecExpr: Expr */
{
	const struct s_Expr	_;
	void				*vec;	/* Vec container */
};

extern const void	*VecExpr;	/* new(VecExpr, NULL, Tag, Vec); */

void	initVecExpr(void);

#endif
