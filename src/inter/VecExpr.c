#include <assert.h>
#include <stdarg.h>

/* container */
#include "Vec.h"

/* inter */
#include "Expr.h"
#include "Expr.r"
#include "VecExpr.h"

/* lexer */
#include "Vector.h"

const void	*VecExpr;

/* VecExpr constructor method. */
static void	*VecExpr_ctor(void *_self, va_list *app)
{
	struct s_VecExpr	*self;

	self = super_ctor(VecExpr, _self, app);
	self->vec = va_arg(*app, void *);
	return (self);
}

/* VecExpr copy constructor method. */
static void	*VecExpr_copy(const void *_self)
{
	const struct s_VecExpr	*self = _self;
	struct s_VecExpr		*retval;

	retval = super_copy(VecExpr, _self);
	retval->vec = copy(self->vec);
	return (retval);
}

/* VecExpr destructor method. */
static void	*VecExpr_dtor(void *_self)
{
	struct s_VecExpr	*self = _self;

	delete(self->vec);
	return (super_dtor(VecExpr, _self));
}

static char	*VecExpr_str(const void *_self)
{
	const struct s_VecExpr	*self = _self;

	return (str(self->vec));
}

/* Evaluate the expression into a Vector class */
static struct s_Token	*VecExpr_eval(const void *_self, void *env)
{
	const struct s_VecExpr	*self = _self;
	size_t					size = Vec_size(self->vec);
	void					*retval = new(Vector, VECTOR, size);
	size_t					i;

	for (i = 0; i < size; ++i)
		Vector_update(retval, i, eval(Vec_at(self->vec, i), env));
	return (retval);
}

void	initVecExpr(void)
{
	initExpr();
	if (!VecExpr)
		VecExpr = new(ExprClass, "VecExpr",
				Expr, sizeof(struct s_VecExpr),
				ctor, VecExpr_ctor,
				copy, VecExpr_copy,
				dtor, VecExpr_dtor,
				str, VecExpr_str,
				eval, VecExpr_eval,
				0);
}
