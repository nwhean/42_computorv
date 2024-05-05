#include <assert.h>

#include "Expr.h"
#include "Expr.r"

const void	*Expr;
const void	*ExprClass;

/* Expr constructor method. */
static void	*Expr_ctor(void *_self, va_list *app)
{
	struct s_Expr	*self;

	self = super_ctor(Expr, _self, app);
	self->op = va_arg(*app, struct s_Token *);
	self->tag = va_arg(*app, enum e_Tag);
	return (self);
}

/* Expr copy constructor method. */
static void	*Expr_copy(void *_self)
{
	struct s_Expr	*self = _self;
	struct s_Expr	*retval;

	retval = (super_copy(Expr, _self));
	retval->op = copy(self->op);
	retval->tag = self->tag;
	return (retval);
}

/* Expr destructor method. */
static void	*Expr_dtor(void *_self)
{
	struct s_Expr	*self = _self;

	delete(self->op);
	return (super_dtor(Expr, _self));
}

static char	*Expr_str(const void *_self)
{
	const struct s_Expr	*self = _self;

	return (str(self->op));
}

static bool	Expr_equal(const void *_self, const void *_other)
{
	const struct s_Expr	*self = _self;
	const struct s_Expr	*other = _other;

	if (self->tag != other->tag)
		return (false);
	return equal(self->op, other->op);
}

/* Return Token representing the Expr. */
struct s_Token	*eval(const void *self, void *env)
{
	const struct s_ExprClass *const	*cp = self;

	assert(self && *cp && (*cp)->eval);
	return ((*cp)->eval(self, env));
}

struct s_Token	*super_eval(const void *_class, const void *_self, void *env)
{
	const struct s_ExprClass	*superclass = super(_class);

	assert(_self && superclass->eval);
	return (superclass->eval(_self, env));
}

static struct s_Token	*Expr_eval(const void *_self, void *env)
{
	const struct s_Expr	*self = _self;

	(void)env;
	return copy(self->op);
}

/* Get the op of an Expr. */
const struct s_Token	*get_op(const void *_self)
{
	const struct s_Expr	*self = _self;

	return (self->op);
}

/* Set the tag of an Expr. */
void	set_tag(void *_self, enum e_Tag tag)
{
	struct s_Expr	*self = _self;

	self->tag = tag;
}

/* Get the tag of an Expr. */
enum e_Tag	get_tag(const void *_self)
{
	const struct s_Expr	*self = _self;

	return (self->tag);
}

/* ExprClass constructor method. */
static void	*ExprClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_ExprClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(ExprClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		method = va_arg(ap, voidf);
		if (selector == (voidf)eval)
			*(voidf *)&self->eval = method;
		#pragma GCC diagnostic pop
	}
	return (self);
}

void	initExpr(void)
{
	initNode();
	if (!ExprClass)
		ExprClass = new(Class, "ExprClass",
				NodeClass, sizeof(struct s_ExprClass),
				ctor, ExprClass_ctor,
				0);
	if (!Expr)
		Expr = new(ExprClass, "Expr",
				Node, sizeof(struct s_Expr),
				ctor, Expr_ctor,
				copy, Expr_copy,
				dtor, Expr_dtor,
				str, Expr_str,
				equal, Expr_equal,
				eval, Expr_eval,
				0);
}
