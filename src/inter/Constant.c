#include <assert.h>
#include <stdarg.h>

#include "Expr.h"
#include "Expr.r"
#include "Constant.h"

const void	*Constant;

/* Constant constructor method. */
static void	*Constant_ctor(void *_self, va_list *app)
{
	struct s_Constant	*self;

	self = super_ctor(Constant, _self, app);
	return (self);
}

/* Constant destructor method. */
static void	*Constant_dtor(void *_self)
{
	struct s_Constant	*self;

	self = super_dtor(Constant, _self);
	return (self);
}

static struct s_Expr	*Constant_gen(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Constant		*self = _self;

	return (super_gen(class, self));
}

static struct s_Expr	*Constant_reduce(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Constant		*self = _self;

	return (super_reduce(class, self));
}

static const char	*Constant_to_string(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Constant		*self = _self;

	return (super_to_string(class, self));
}

static const struct s_Token	*Constant_eval(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Constant		*self = _self;

	return (super_eval(class, self));
}

void	initConstant(void)
{
	initExpr();
	if (!Constant)
		Constant = new(ExprClass, "Constant",
				Expr, sizeof(struct s_Constant),
				ctor, Constant_ctor,
				dtor, Constant_dtor,
				gen, Constant_gen,
				reduce, Constant_reduce,
				to_string, Constant_to_string,
				eval, Constant_eval,
				0);
}
