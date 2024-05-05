#include <stdarg.h>

/* inter */
#include "Expr.h"
#include "Expr.r"
#include "Constant.h"

/* symbols */
#include "Env.h"

const void	*Constant;

/* Constant constructor method. */
static void	*Constant_ctor(void *_self, va_list *app)
{
	struct s_Constant	*self;

	self = super_ctor(Constant, _self, app);
	return (self);
}

/* Constant copy constructor method. */
static void	*Constant_copy(const void *_self)
{
	return (super_copy(Constant, _self));
}

/* Constant destructor method. */
static void	*Constant_dtor(void *_self)
{
	struct s_Constant	*self;

	self = super_dtor(Constant, _self);
	return (self);
}

static char	*Constant_str(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Constant		*self = _self;

	return (super_str(class, self));
}

static struct s_Token	*Constant_eval(const void *_self, void *env)
{
	(void)env;
	return (copy(get_op(_self)));
}

void	initConstant(void)
{
	initExpr();
	if (!Constant)
		Constant = new(ExprClass, "Constant",
				Expr, sizeof(struct s_Constant),
				ctor, Constant_ctor,
				copy, Constant_copy,
				dtor, Constant_dtor,
				str, Constant_str,
				eval, Constant_eval,
				0);
}
