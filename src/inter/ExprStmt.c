/* inter */
#include "ExprStmt.h"

/* lexer */
#include "Token.h"
#include "Expr.h"

const void	*ExprStmt;

/* ExprStmt constructor method. */
static void	*ExprStmt_ctor(void *_self, va_list *app)
{
	struct s_ExprStmt	*self;

	self = super_ctor(ExprStmt, _self, app);
	self->expr = va_arg(*app, struct s_Expr *);
	return (self);
}

/* ExprStmt destructor method. */
static void	*ExprStmt_dtor(void *_self)
{
	struct s_ExprStmt	*self = _self;

	delete(self->expr);
	return (super_dtor(ExprStmt, _self));
}

static char	*ExprStmt_str(const void *_self)
{
	const struct s_ExprStmt	*self = _self;

	return (str(self->expr));
}

static void	ExprStmt_exec(const void *_self, void *env)
{
	const struct s_ExprStmt	*self = _self;
	char					*s = str(self->expr);
	struct s_Token			*token;

	/* print the expression */
	printf("%s\n", s);
	free(s);

	/* evaluate the expression and print result */
	token = eval(self->expr, env);
	if (token)
	{
		char	*result = str(token);

		printf("%s\n", result);
		free(result);
		if (token->tag != ID)
			delete(token);
	}
}

void	initExprStmt(void)
{
	initStmt();
	if (!ExprStmt)
		ExprStmt = new(StmtClass, "ExprStmt",
				Stmt, sizeof(struct s_ExprStmt),
				ctor, ExprStmt_ctor,
				dtor, ExprStmt_dtor,
				str, ExprStmt_str,
				exec, ExprStmt_exec,
				0);
}
