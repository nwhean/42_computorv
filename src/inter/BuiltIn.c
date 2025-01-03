#include <stdarg.h>

/* container */
#include "Str.h"
#include "Vec.h"

/* inter */
#include "Expr.h"
#include "Expr.r"
#include "BuiltIn.h"

/* lexer */
#include "Word.h"

/* other */
#include "mathematics.h"
#include "utility.h"

/* symbols */
#include "Env.h"

const void	*BuiltIn;
const void	*BuiltInFunc;

typedef void	*(*fptr)(const void *, const void *);

/* BuiltIn constructor method. */
static void	*BuiltIn_ctor(void *_self, va_list *app)
{
	struct s_BuiltIn	*self = _self;

	self = super_ctor(BuiltIn, _self, app);
	self->count = va_arg(*app, size_t);
	self->fptr = va_arg(*app, fptr);
	return (self);

}

/* BuiltIn copy constructor method. */
static void	*BuiltIn_copy(const void *_self)
{
	const struct s_BuiltIn	*self = _self;
	struct s_BuiltIn		*retval;

	retval = super_copy(BuiltIn, _self);
	retval->count = self->count;
	retval->fptr = self->fptr;
	return (retval);
}

/* BuiltIn destructor method. */
static void	*BuiltIn_dtor(void *_self)
{
	return (super_dtor(BuiltIn, _self));
}

static char	*BuiltIn_str(const void *_self)
{
	const struct s_BuiltIn		*self = _self;
	void						*s;
	char						*s_append;
	char						*retval;

	/* opening bracket */
	s = new(Str, "<built-in function ");

	/* insert name */
	s_append = str(get_op(self));
	Str_append(s, s_append);
	free(s_append);

	/* closing bracket */
	Str_push_back(s, '>');

	retval = str(s);
	delete(s);
	return (retval);
}

/* call a built in function */
void	*BuiltIn_call(const void *_self, const void *params, const void *env)
{
	const struct s_BuiltIn	*self = _self;
	size_t					size = Vec_size(params);

	/* verify that the number of parameters is as expected */
	if (size != self->count)
	{
		fprintf(stderr, "BuiltIn_call: expect %ld arguments, get %ld\n",
				self->count, size);
		return (NULL);
	}
	return (self->fptr(params, env));
}

/* add a built-in function to the environment. */
static void	BuiltIn_add(const struct s_BuiltIn *func)
{
	Env_put((void *)BuiltInFunc, copy(get_op(func)), (void *)func);
}

const struct s_BuiltIn	*BuiltIn_exit;
const struct s_BuiltIn	*BuiltIn_exp;
const struct s_BuiltIn	*BuiltIn_ln;
const struct s_BuiltIn	*BuiltIn_sin;
const struct s_BuiltIn	*BuiltIn_cos;
const struct s_BuiltIn	*BuiltIn_tan;
const struct s_BuiltIn	*BuiltIn_sinh;
const struct s_BuiltIn	*BuiltIn_cosh;
const struct s_BuiltIn	*BuiltIn_tanh;
const struct s_BuiltIn	*BuiltIn_sqrt;
const struct s_BuiltIn	*BuiltIn_abs;
const struct s_BuiltIn	*BuiltIn_radians;
const struct s_BuiltIn	*BuiltIn_degrees;
const struct s_BuiltIn	*BuiltIn_norm;
const struct s_BuiltIn	*BuiltIn_inv;
const struct s_BuiltIn	*BuiltIn_vars;

void	initBuiltIn(void)
{
	initStr();
	initVec();
	initExpr();
	initWord();
	if (!BuiltIn)
	{
		BuiltIn = new(ExprClass, "BuiltIn",
				Expr, sizeof(struct s_BuiltIn),
				ctor, BuiltIn_ctor,
				copy, BuiltIn_copy,
				dtor, BuiltIn_dtor,
				str, BuiltIn_str,
				0);
		BuiltInFunc = new(Env, NULL);

		/* define built-in functions */
		BuiltIn_exit = new(BuiltIn, new(Word, FUNCTION, "exit"), BUILTIN,
						0, ft_exit);
		BuiltIn_exp = new(BuiltIn, new(Word, FUNCTION, "exp"), BUILTIN,
						1, ft_exp);
		BuiltIn_ln = new(BuiltIn, new(Word, FUNCTION, "ln"), BUILTIN,
						1, ft_ln);
		BuiltIn_sin = new(BuiltIn, new(Word, FUNCTION, "sin"), BUILTIN,
						1, ft_sin);
		BuiltIn_cos = new(BuiltIn, new(Word, FUNCTION, "cos"), BUILTIN,
						1, ft_cos);
		BuiltIn_tan = new(BuiltIn, new(Word, FUNCTION, "tan"), BUILTIN,
						1, ft_tan);
		BuiltIn_sinh = new(BuiltIn, new(Word, FUNCTION, "sinh"), BUILTIN,
						1, ft_sinh);
		BuiltIn_cosh = new(BuiltIn, new(Word, FUNCTION, "cosh"), BUILTIN,
						1, ft_cosh);
		BuiltIn_tanh = new(BuiltIn, new(Word, FUNCTION, "tanh"), BUILTIN,
						1, ft_tanh);
		BuiltIn_sqrt = new(BuiltIn, new(Word, FUNCTION, "sqrt"), BUILTIN,
						1, ft_sqrt);
		BuiltIn_abs = new(BuiltIn, new(Word, FUNCTION, "abs"), BUILTIN,
						1, ft_abs);
		BuiltIn_radians = new(BuiltIn, new(Word, FUNCTION, "radians"), BUILTIN,
						1, ft_radians);
		BuiltIn_degrees = new(BuiltIn, new(Word, FUNCTION, "degrees"), BUILTIN,
						1, ft_degrees);
		BuiltIn_norm = new(BuiltIn, new(Word, FUNCTION, "norm"), BUILTIN,
						1, ft_norm);
		BuiltIn_inv = new(BuiltIn, new(Word, FUNCTION, "inv"), BUILTIN,
						1, ft_inv);
		BuiltIn_vars = new(BuiltIn, new(Word, FUNCTION, "vars"), BUILTIN,
						0, ft_vars);

		BuiltIn_add(BuiltIn_exit);
		BuiltIn_add(BuiltIn_exp);
		BuiltIn_add(BuiltIn_ln);
		BuiltIn_add(BuiltIn_sin);
		BuiltIn_add(BuiltIn_cos);
		BuiltIn_add(BuiltIn_tan);
		BuiltIn_add(BuiltIn_sinh);
		BuiltIn_add(BuiltIn_cosh);
		BuiltIn_add(BuiltIn_tanh);
		BuiltIn_add(BuiltIn_sqrt);
		BuiltIn_add(BuiltIn_abs);
		BuiltIn_add(BuiltIn_radians);
		BuiltIn_add(BuiltIn_degrees);
		BuiltIn_add(BuiltIn_norm);
		BuiltIn_add(BuiltIn_inv);
		BuiltIn_add(BuiltIn_vars);
	}
}
