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

/* Expr destructor method. */
static void	*Expr_dtor(void *_self)
{
	struct s_Expr	*self = _self;

	delete(self->op);
	return (super_dtor(Expr, _self));
}

struct s_Expr	*gen(const void *self)
{
	const struct s_ExprClass *const	*cp = self;

	assert(self && *cp && (*cp)->gen);
	return ((*cp)->gen(self));
}

struct s_Expr	*super_gen(const void *_class, const void *_self)
{
	const struct s_ExprClass	*superclass = super(_class);

	assert(_self && superclass->gen);
	return (superclass->gen(_self));
}

static struct s_Expr	*Expr_gen(const void *self)
{
	return ((struct s_Expr *)self);
}

struct s_Expr	*reduce(const void *self)
{
	const struct s_ExprClass *const	*cp = self;

	assert(self && *cp && (*cp)->reduce);
	return ((*cp)->reduce(self));
}

struct s_Expr	*super_reduce(const void *_class, const void *_self)
{
	const struct s_ExprClass	*superclass = super(_class);

	assert(_self && superclass->reduce);
	return (superclass->reduce(_self));
}

static struct s_Expr	*Expr_reduce(const void *self)
{
	return ((struct s_Expr *)self);
}

/* Return string representing the Expr. */
const char	*to_string(const void *self)
{
	const struct s_ExprClass *const	*cp = self;

	assert(self && *cp && (*cp)->to_string);
	return ((*cp)->to_string(self));
}

const char	*super_to_string(const void *_class, const void *_self)
{
	const struct s_ExprClass	*superclass = super(_class);

	assert(_self && superclass->to_string);
	return (superclass->to_string(_self));
}

static const char	*Expr_to_string(const void *_self)
{
	const struct s_Expr	*self = _self;

	return (token_to_string(self->op));
}

/* Return Token representing the Expr. */
const struct s_Token	*eval(const void *self)
{
	const struct s_ExprClass *const	*cp = self;

	assert(self && *cp && (*cp)->eval);
	return ((*cp)->eval(self));
}

const struct s_Token	*super_eval(const void *_class, const void *_self)
{
	const struct s_ExprClass	*superclass = super(_class);

	assert(_self && superclass->eval);
	return (superclass->eval(_self));
}

static const struct s_Token	*Expr_eval(const void *_self)
{
	const struct s_Expr	*self = _self;

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
		if (selector == (voidf)gen)
			*(voidf *)&self->gen = method;
		else if (selector == (voidf)reduce)
			*(voidf *)&self->reduce = method;
		else if (selector == (voidf)to_string)
			*(voidf *)&self->to_string = method;
		else if (selector == (voidf)eval)
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
				dtor, Expr_dtor,
				gen, Expr_gen,
				reduce, Expr_reduce,
				to_string, Expr_to_string,
				eval, Expr_eval,
				0);
}
