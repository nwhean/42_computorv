#include "Stmt.h"

const void	*Stmt;
const void	*StmtClass;

/* Stmt constructor method. */
static void	*Stmt_ctor(void *_self, va_list *app)
{
	struct s_Stmt	*self;

	self = super_ctor(Stmt, _self, app);
	return (self);
}

void	initStmt(void)
{
	initNode();
	if (!Stmt)
		Stmt = new(NodeClass, "Stmt",
				Node, sizeof(struct s_Stmt),
				ctor, Stmt_ctor,
				0);
}
