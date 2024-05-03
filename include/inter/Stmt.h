#ifndef STMT_H
# define STMT_H

# include "Node.h"
# include "Node.r"

extern const void	*Stmt;	/* new(Stmt); */

/* Base class from which Stmt subclasses derives from. */
struct s_Stmt	/* Stmt: Node */
{
	const struct s_Node	_;
};

void	initStmt(void);

#endif
