#include <stdarg.h>

/* container */
#include "Str.h"
#include "Vec.h"

/* inter */
#include "Expr.h"
#include "Expr.r"
#include "Function.h"

/* symbols */
#include "Env.h"

const void	*Function;

/* Function constructor method. */
static void	*Function_ctor(void *_self, va_list *app)
{
	struct s_Function	*self = _self;

	self = super_ctor(Function, _self, app);
	self->params = va_arg(*app, void *);	/* Vec */
	self->expr = NULL;

	if (!get_op(self) || get_tag(self) != FUNCTION || !self->params)
	{
		fprintf(stderr, "Function_ctor: Invalid member variable.\n");
		delete(self);
		return (NULL);
	}
	return (self);

}

/* Function copy constructor method. */
static void	*Function_copy(const void *_self)
{
	const struct s_Function	*self = _self;
	struct s_Function		*retval;

	retval = super_copy(Function, _self);
	retval->params = copy(self->params);
	retval->expr = self->expr ? copy(self->expr) : NULL;
	return (retval);
}

/* Function destructor method. */
static void	*Function_dtor(void *_self)
{
	struct s_Function	*self = _self;

	delete(self->params);
	delete(self->expr);
	self = super_dtor(Function, _self);
	return (self);
}

static char	*Function_str(const void *_self)
{
	const struct s_Function		*self = _self;
	void						*s = new(Str, "");
	char						*s_append;
	char						*retval;
	size_t						size;
	size_t						i;

	/* insert name */
	s_append = str(get_op(self));
	Str_append(s, s_append);
	free(s_append);

	/* insert parameters */
	Str_push_back(s, '(');
	size = Vec_size(self->params);
	for (i = 0; i < size; ++i)
	{
		s_append = str(Vec_at(self->params, i));
		Str_append(s, s_append);
		free(s_append);
		if (i < size - 1)
			Str_append(s, ", ");
	}
	Str_push_back(s, ')');

	retval = str(s);
	delete(s);
	return (retval);
}

static struct s_Token	*Function_eval(const void *_self, void *env)
{
	const struct s_Function		*self = _self;

	return (eval(self->expr, env));
}

void	initFunction(void)
{
	initStr();
	initVec();
	initExpr();
	if (!Function)
		Function = new(ExprClass, "Function",
				Expr, sizeof(struct s_Function),
				ctor, Function_ctor,
				copy, Function_copy,
				dtor, Function_dtor,
				str, Function_str,
				eval, Function_eval,
				0);
}
