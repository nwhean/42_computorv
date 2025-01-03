#include <assert.h>
#include <stdarg.h>

/* container */
#include "Str.h"

/* inter */
#include "Expr.r"
#include "Op.h"
#include "Unary.h"

/* lexer */
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

/* Unary copy constructor method. */
static void	*Unary_copy(const void *_self)
{
	const struct s_Unary	*self = _self;
	struct s_Unary			*retval;

	retval = super_copy(Unary, _self);
	retval->expr = copy(self->expr);
	return (retval);
}

/* Unary destructor method. */
static void	*Unary_dtor(void *_self)
{
	struct s_Unary	*self = _self;

	delete(self->expr);
	self = super_dtor(Unary, _self);
	return (self);
}

/* Return the string representation in Reverse Polish Notation. */
static char	*Unary_str(const void *_self)
{
	const struct s_Unary	*self = _self;
	const char				*str1 = str(get_op(self));
	const char				*str2 = str(self->expr);
	void					*s = new(Str, str1);
	char					*retval;

	Str_push_back(s, ' ');
	Str_append(s, str2);
	retval = str(s);
	free((char *)str1);
	free((char *)str2);
	delete(s);
	return (retval);
}

static struct s_Token	*Unary_eval(const void *_self, void *env)
{
	const struct s_Unary	*self = _self;
	void					*expr = eval(self->expr, env);

	if (!expr)
		return (NULL);
	if (get_op(self) == (struct s_Token *)Word_minus)
		numeric_ineg(&expr);
	return (expr);
}

/* Convert Expr to a Polynomial */
static struct s_Token	*Unary_to_polynomial(const void *self, void *env)
{
	return (super_to_polynomial(Unary, self, env));
}

void	initUnary(void)
{
	initStr();
	initOp();
	if (!Unary)
		Unary = new(ExprClass, "Unary",
				Op, sizeof(struct s_Unary),
				ctor, Unary_ctor,
				copy, Unary_copy,
				dtor, Unary_dtor,
				str, Unary_str,
				eval, Unary_eval,
				to_polynomial, Unary_to_polynomial,
				0);
}
