/* container */
#include "Str.h"

/* inter */
#include "Constant.h"
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

	if (get_tag(self->id) == ID && self->expr)
		return (self);
	if (get_tag(self->id) != ID)
		fprintf(stderr, "AssignStmt_ctor: expect Id as LHS, get '%c'.\n",
				get_tag(self->id));
	if (!self->expr)
		fprintf(stderr, "AssignStmt_ctor: RHS is ill-defined.\n");
	delete(self);
	return (NULL);
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
		Env_put(env, copy(get_op(self->id)), new(Constant, token, ID));
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
