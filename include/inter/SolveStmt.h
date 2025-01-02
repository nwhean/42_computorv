#ifndef SOLVESTMT_H
# define SOLVESTMT_H

# include "Stmt.h"

/* Expression Statement class */
struct s_SolveStmt	/* SolveStmt: Stmt */
{
	const struct s_Stmt	_;
	struct s_Expr		*lhs;
	struct s_Expr		*rhs;
};

extern const void	*SolveStmt;	/* new(SolveStmt, Expr, Expr); */

void	initSolveStmt(void);

#endif
