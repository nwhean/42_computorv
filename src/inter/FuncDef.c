/* container */
#include "Str.h"
#include "Vec.h"

/* inter */
#include "Expr.h"
#include "FuncDef.h"
#include "Function.h"

/* lexer */
#include "Token.h"

/* symbols */
#include "Env.h"

const void	*FuncDef;

/* FuncDef constructor method. */
static void	*FuncDef_ctor(void *_self, va_list *app)
{
	struct s_FuncDef	*self;

	self = super_ctor(FuncDef, _self, app);
	self->func = va_arg(*app, struct s_Function *);
	self->expr = va_arg(*app, struct s_Expr *);

	if (get_tag(self->func) == FUNCTION && self->expr)
		return (self);
	if (get_tag(self->func) != FUNCTION)
		fprintf(stderr, "FuncDef_ctor: expect Function as LHS, get '%c'.\n",
				get_tag(self->func));
	if (!self->expr)
		fprintf(stderr, "FuncDef_ctor: RHS is ill-defined.\n");
	delete(self);
	return (NULL);
}

/* FuncDef destructor method. */
static void	*FuncDef_dtor(void *_self)
{
	struct s_FuncDef	*self = _self;

	delete(self->func);
	delete(self->expr);
	return (super_dtor(FuncDef, _self));
}

/* Return a string representation of the FuncDef. */
static char	*FuncDef_str(const void *_self)
{
	const struct s_FuncDef	*self = _self;
	void					*s = new(Str, "");
	char					*s_append;
	char					*retval;

	/* insert function */
	s_append = str(self->func);
	Str_append(s, s_append);
	free(s_append);

	Str_append(s, " = ");

	/* insert expr */
	s_append = str(self->expr);
	Str_append(s, s_append);
	free(s_append);

	retval = str(s);
	delete(s);
	return (retval);
}

/* Put the function definition into the environment */
static void	FuncDef_exec(void *_self, void *env)
{
	struct s_FuncDef	*self = _self;
	char				*s;
	void				*params;	/* Vec of Expr */
	size_t				size;
	size_t				i;
	size_t				j;

	/* check that function is valid */
	if (!self->func)
	{
		fprintf(stderr, "FuncDef_exec: function is invalid.\n");
		return ;
	}

	/* check that expression is valid */
	if (!self->expr)
	{
		fprintf(stderr, "FuncDef_exec: expression is invalid.\n");
		return ;
	}

	/* check that parameters are unique */
	params = self->func->params;
	size = Vec_size(params);
	for (i = 0; i < size; ++i)
	{
		if (get_tag(Vec_at(params, i)) != ID)
		{
			fprintf(stderr, "FuncDef_exec: argument must be an ID.\n");
			return ;
		}
		for (j = i + 1; j < size; ++j)
		{
			if (equal(Vec_at(params, i), Vec_at(params, j)))
			{
				fprintf(stderr, "FuncDef_exec: duplicate argument.\n");
				return ;
			}
		}
	}

	/* print input string */
	s = str(self);
	printf("%s\n", s);
	free(s);

	/* print the expr */
	s = str(self->expr);
	printf("%s\n", s);
	free(s);

	/* put the function into the environment */
	self->func->expr = self->expr;
	Env_put(env, copy(get_op(self->func)), self->func);
	self->func = NULL;
	self->expr = NULL;
}

void	initFuncDef(void)
{
	initStr();
	initVec();
	initStmt();
	initEnv();
	if (!FuncDef)
		FuncDef = new(StmtClass, "FuncDef",
				Stmt, sizeof(struct s_FuncDef),
				ctor, FuncDef_ctor,
				dtor, FuncDef_dtor,
				str, FuncDef_str,
				exec, FuncDef_exec,
				0);
}
