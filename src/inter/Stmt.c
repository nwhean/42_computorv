#include <assert.h>

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

/* Return Token representing the Expr. */
void	exec(const void *self, void *env)
{
	const struct s_StmtClass *const	*cp = self;

	assert(self && *cp && (*cp)->exec);
	(*cp)->exec(self, env);
}

void	super_exec(const void *_class, const void *_self, void *env)
{
	const struct s_StmtClass	*superclass = super(_class);

	assert(_self && superclass->exec);
	superclass->exec(_self, env);
}

/* StmtClass constructor method. */
static void	*StmtClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_StmtClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(StmtClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		method = va_arg(ap, voidf);
		if (selector == (voidf)exec)
			*(voidf *)&self->exec = method;
		#pragma GCC diagnostic pop
	}
	return (self);
}

void	initStmt(void)
{
	initNode();
	if (!StmtClass)
		StmtClass = new(Class, "StmtClass",
				NodeClass, sizeof(struct s_StmtClass),
				ctor, StmtClass_ctor,
				0);
	if (!Stmt)
		Stmt = new(NodeClass, "Stmt",
				Node, sizeof(struct s_Stmt),
				ctor, Stmt_ctor,
				0);
}
