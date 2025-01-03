#include <stdarg.h>

/* container */
#include "Str.h"
#include "Vec.h"

/* inter */
#include "BuiltIn.h"
#include "Constant.h"
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

	/* insert expr if available */
	if (self->expr)
	{
		Str_append(s, " = ");
		s_append = str(self->expr);
		Str_append(s, s_append);
		free(s_append);
	}

	retval = str(s);
	delete(s);
	return (retval);
}

static struct s_Token	*Function_eval(const void *_self, void *env)
{
	const struct s_Function	*self = _self;
	const struct s_Function	*def = Env_get(env, get_op(self));
	void					*env_new;
	size_t					size = Vec_size(self->params);
	size_t					i;
	void					*val;
	struct s_Token			*retval = NULL;

	if (!def)
	{
		/* check whether there's a built-in function */
		void	*built_in = Env_get(BuiltInFunc, get_op(self));

		if (built_in)
			return (BuiltIn_call(built_in, self->params, env));

		fprintf(stderr, "Function_eval: undefined function.\n");
		return (NULL);
	}

	/* create a new environment to evaluate the function expression */
	env_new = new(Env, env);
	for (i = 0; i < size; ++i)
	{
		/* evaluate input parameter with the current environment */
		val = eval(Vec_at(self->params, i), env);

		if (!val)
		{
			fprintf(stderr, "argument cannot be NULL\n");
			break ;
		}

		Env_put(
			env_new,
			copy(get_op(Vec_at(def->params, i))),
			new(Constant, val, get_tag(val)));
	}

	/* evaluate the expression using the newly defined environment */
	if (val)
		retval = eval(def->expr, env_new);
	delete(env_new);
	return (retval);
}

/* Convert Function to a Polynomial */
static struct s_Token	*Function_to_polynomial(const void *_self, void *env)
{
	const struct s_Function	*self = _self;
	const struct s_Function	*def = Env_get(env, get_op(self));
	void					*env_new;
	size_t					size = Vec_size(self->params);
	size_t					i;
	void					*val;
	struct s_Token			*retval = NULL;

	if (size > 1)
	{
		fprintf(stderr, "Function_to_polynomial: too many parameters.\n");
		return (NULL);
	}

	if (!def)
	{
		/* check whether there's a built-in function */
		void	*built_in = Env_get(BuiltInFunc, get_op(self));

		if (built_in)
			return (BuiltIn_call(built_in, self->params, env));

		fprintf(stderr, "Function_to_polynomial: undefined function.\n");
		return (NULL);
	}

	/* create a new environment to evaluate the function expression */
	env_new = new(Env, env);
	for (i = 0; i < size; ++i)
	{
		/* evaluate input parameter with the current environment */
		void	*param_self = Vec_at(self->params, i);
		void	*param_def = Vec_at(def->params, i);

		val = eval(param_self, env);
		if (!val)								/* if it is an unknown */
		{
			if (!equal(param_self, param_def))	/* update parame if different*/
				Env_put(
					env_new,
					copy(get_op(param_def)),
					copy(param_self)
					);
		}
		else
			Env_put(
				env_new,
				copy(get_op(param_def)),
				new(Constant, val, get_tag(val))
				);
	}

	retval = to_polynomial(def->expr, env_new);
	delete(env_new);
	return (retval);
}

void	initFunction(void)
{
	initStr();
	initVec();
	initBuiltIn();
	initConstant();
	initExpr();
	if (!Function)
		Function = new(ExprClass, "Function",
				Expr, sizeof(struct s_Function),
				ctor, Function_ctor,
				copy, Function_copy,
				dtor, Function_dtor,
				str, Function_str,
				eval, Function_eval,
				to_polynomial, Function_to_polynomial,
				0);
}
