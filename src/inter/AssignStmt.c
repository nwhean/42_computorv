/* container */
#include "Str.h"

/* inter */
#include "Expr.h"
#include "AssignStmt.h"

/* lexer */
#include "Token.h"

/* symbols */
#include "Env.h"

const void	*AssignStmt;

/* AssignStmt constructor method. */
static void	*AssignStmt_ctor(void *_self, va_list *app)
{
	struct s_AssignStmt	*self;

	self = super_ctor(AssignStmt, _self, app);
	self->id = va_arg(*app, struct s_Id *);
	self->expr = va_arg(*app, struct s_Expr *);
	return (self);
}

/* AssignStmt destructor method. */
static void	*AssignStmt_dtor(void *_self)
{
	struct s_AssignStmt	*self = _self;

	delete(self->id);
	delete(self->expr);
	return (super_dtor(AssignStmt, _self));
}

static char	*AssignStmt_str(const void *_self)
{
	const struct s_AssignStmt	*self = _self;
	void						*s = new(Str, "");
	char						*s_append;
	char						*retval;

	s_append = str(self->id);
	Str_append(s, s_append);
	free(s_append);
	Str_append(s, " = ");
	s_append = str(self->expr);
	Str_append(s, s_append);
	free(s_append);
	retval = str(s);
	delete(s);
	return (retval);
}

/* Put the id and expr in the Environment*/
static void	AssignStmt_exec(void *_self, void *env)
{
	struct s_AssignStmt	*self = _self;
	char				*s;
	void				*token;

	/* print input string */
	s = str(_self);
	printf("%s\n", s);
	free(s);

	token = eval(self->expr, env);
	if (token)
	{
		s = str(token);
		printf("%s\n", s);
		free(s);

		/* put key-value pair in the environment */
		Env_put(env, self->id, token);
		self->id = NULL;	/* prevent deletion of id */
	}
	else
		fprintf(stderr, "Assignment failure\n");
}

void	initAssignStmt(void)
{
	initStr();
	initStmt();
	initEnv();
	if (!AssignStmt)
		AssignStmt = new(StmtClass, "AssignStmt",
				Stmt, sizeof(struct s_AssignStmt),
				ctor, AssignStmt_ctor,
				dtor, AssignStmt_dtor,
				str, AssignStmt_str,
				exec, AssignStmt_exec,
				0);
}
