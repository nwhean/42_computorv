#ifndef FUNCDEF_H
# define FUNCDEF_H

/* inter */
# include "Stmt.h"

/* Expression Statement class */
struct s_FuncDef	/* FuncDef: Stmt */
{
	const struct s_Stmt	_;
	struct s_Function	*func;
	struct s_Expr		*expr;
};

extern const void	*FuncDef;	/* new(FuncDef, Function, Expr); */

void	initFuncDef(void);

#endif
