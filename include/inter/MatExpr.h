#ifndef MATEXPR_H
# define MATEXPR_H

# include "Expr.h"

struct s_MatExpr	/* MatExpr: Expr */
{
	const struct s_Expr	_;
	size_t				rows;
	size_t				cols;
	void				*vec;	/* Vec container */
};

extern const void	*MatExpr;	/* new(MatExpr, NULL, Tag, Vec); */

void	initMatExpr(void);

#endif
