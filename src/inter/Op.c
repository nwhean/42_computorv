#include <assert.h>
#include <stdarg.h>

#include "Expr.h"
#include "Expr.r"
#include "Op.h"

const void	*Op;

/* Op constructor method. */
static void	*Op_ctor(void *_self, va_list *app)
{
	struct s_Op	*self;

	self = super_ctor(Op, _self, app);
	return (self);
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

static const struct s_Token	*Op_eval(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Op			*self = _self;

	return (super_eval(class, self));
}

void	initOp(void)
{
	initExpr();
	if (!Op)
		Op = new(ExprClass, "Op",
				Expr, sizeof(struct s_Op),
				ctor, Op_ctor,
				dtor, Op_dtor,
				str, Op_str,
				eval, Op_eval,
				0);
}
