#ifndef ASSIGNSTMT_H
# define ASSIGNSTMT_H

# include "Stmt.h"
# include "Word.h"

/* Expression Statement class */
struct s_AssignStmt	/* AssignStmt: Stmt */
{
	const struct s_Stmt	_;
	struct s_Id			*id;
	struct s_Expr		*expr;
};

extern const void	*AssignStmt;	/* new(AssignStmt, Id, Expr); */

void	initAssignStmt(void);

#endif
