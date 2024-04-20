#include <assert.h>
#include <stdarg.h>

#include "Expr.h"
#include "Expr.r"
#include "Id.h"
#include "Word.h"

const void	*Id;

/* Id constructor method. */
static void	*Id_ctor(void *_self, va_list *app)
{
	struct s_Id	*self;

	self = super_ctor(Id, _self, app);
	return (self);
}

/* Id destructor method. */
static void	*Id_dtor(void *_self)
{
	struct s_Id	*self;

	self = super_dtor(Id, _self);
	return (self);
}

static struct s_Expr	*Id_gen(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Id			*self = _self;

	return (super_gen(class, self));
}

static struct s_Expr	*Id_reduce(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Id			*self = _self;

	return (super_reduce(class, self));
}

static char	*Id_str(const void *_self)
{
	const struct s_Id			*self = _self;
	const struct s_Work			*token = (struct s_Work *)get_op(self);

	return (str(token));
}

static const struct s_Token	*Id_eval(const void *_self)
{
	const struct s_Id		*self = _self;

	return (get_op(self));
}

void	initId(void)
{
	initExpr();
	if (!Id)
		Id = new(ExprClass, "Id",
				Expr, sizeof(struct s_Id),
				ctor, Id_ctor,
				dtor, Id_dtor,
				str, Id_str,
				gen, Id_gen,
				reduce, Id_reduce,
				eval, Id_eval,
				0);
}
