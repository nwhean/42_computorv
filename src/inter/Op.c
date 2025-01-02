#include <assert.h>
#include <stdarg.h>

/* inter */
#include "Expr.h"
#include "Expr.r"
#include "Op.h"

/* lexer */
#include "Polynomial.h"

const void	*Op;

/* Op constructor method. */
static void	*Op_ctor(void *_self, va_list *app)
{
	struct s_Op	*self;

	self = super_ctor(Op, _self, app);
	return (self);
}

/* Op copy constructor method. */
static void	*Op_copy(const void *_self)
{
	return (super_copy(Op, _self));
}

/* Op destructor method. */
static void	*Op_dtor(void *_self)
{
	struct s_Op	*self;

	self = super_dtor(Op, _self);
	return (self);
}

static char	*Op_str(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Op			*self = _self;

	return (super_str(class, self));
}

static struct s_Token	*Op_eval(const void *_self, void *env)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Op			*self = _self;

	return (super_eval(class, self, env));
}

static struct s_Token	*Op_to_polynomial(const void *self, void *env)
{
	void	*ans;
	void	*retval;

	ans = eval(self, env);
	retval = new(Polynomial, POLYNOMIAL);
	Polynomial_update(retval, 0, ans);
	return (retval);
}

void	initOp(void)
{
	initExpr();
	if (!Op)
		Op = new(ExprClass, "Op",
				Expr, sizeof(struct s_Op),
				ctor, Op_ctor,
				copy, Op_copy,
				dtor, Op_dtor,
				str, Op_str,
				eval, Op_eval,
				to_polynomial, Op_to_polynomial,
				0);
}
