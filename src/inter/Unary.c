#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "Expr.r"
#include "Op.h"
#include "Unary.h"
#include "Numeric.h"
#include "Word.h"

const void	*Unary;

/* Unary constructor method. */
static void	*Unary_ctor(void *_self, va_list *app)
{
	struct s_Unary	*self;

	self = super_ctor(Unary, _self, app);
	self->expr = va_arg(*app, struct s_Expr *);
	set_tag(self, numeric_max(RATIONAL, self->expr->tag));
	assert(get_tag(self));
	return (self);
}

/* Unary destructor method. */
static void	*Unary_dtor(void *_self)
{
	struct s_Unary	*self = _self;

	delete(self->expr);
	self = super_dtor(Unary, _self);
	return (self);
}

static struct s_Expr	*Unary_gen(const void *_self)
{
	const struct s_Unary		*self = _self;

	return (new(Unary, get_op(self), ZERO, reduce(self->expr)));
}

static struct s_Expr	*Unary_reduce(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Unary		*self = _self;

	return (super_reduce(class, self));
}

/* Return the string representation in Reverse Polish Notation. */
static const char	*Unary_to_string(const void *_self)
{
	const struct s_Unary	*self = _self;
	const char				*str1 = token_to_string(get_op(self));
	const char				*str2 = to_string(self->expr);
	size_t					len1 = strlen(str1);
	size_t					len2 = strlen(str2);
	char					*retval;

	retval = calloc(sizeof(char), len1 + len2 + 2);
	if (retval)
	{
		memcpy(retval, str1, len1);
		retval[len1] = ' ';
		memcpy(retval + len1 + 1, str2, len2);
	}
	free((char *)str1);
	free((char *)str2);
	return (retval);
}

static const struct s_Token	*Unary_eval(const void *_self)
{
	const struct s_Unary	*self = _self;
	const struct s_Token	*expr = eval(self->expr);
	struct s_Token			*retval = NULL;

	if (!expr)
		return (NULL);
	if (get_op(self) == (struct s_Token *)Word_plus)
		retval = numeric_pos(expr);
	else
		retval = numeric_neg(expr);
	delete((void *)expr);
	return (retval);
}

void	initUnary(void)
{
	initOp();
	if (!Unary)
		Unary = new(ExprClass, "Unary",
				Op, sizeof(struct s_Unary),
				ctor, Unary_ctor,
				dtor, Unary_dtor,
				gen, Unary_gen,
				reduce, Unary_reduce,
				to_string, Unary_to_string,
				eval, Unary_eval,
				0);
}
