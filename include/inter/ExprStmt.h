#ifndef EXPRSTMT_H
# define EXPRSTMT_H

# include "Stmt.h"

/* Expression Statement class */
struct s_ExprStmt	/* ExprStmt: Stmt */
{
	const struct s_Stmt	_;
	struct s_Expr		*expr;
};

extern const void	*ExprStmt;	/* new(ExprStmt, Expr); */

void	initExprStmt(void);

#endif
