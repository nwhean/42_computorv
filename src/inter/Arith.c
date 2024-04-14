#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "Arith.h"
#include "Expr.r"
#include "Op.h"
#include "Numeric.h"

const void	*Arith;

/* Arith constructor method. */
static void	*Arith_ctor(void *_self, va_list *app)
{
	struct s_Arith	*self;

	self = super_ctor(Arith, _self, app);
	self->expr1 = va_arg(*app, struct s_Expr *);
	self->expr2 = va_arg(*app, struct s_Expr *);
	set_tag(self, numeric_max(self->expr1->tag, self->expr2->tag));
	assert(get_tag(self));
	return (self);
}

/* Arith destructor method. */
static void	*Arith_dtor(void *_self, va_list *app)
{
	struct s_Arith	*self = _self;

	delete(self->expr1);
	delete(self->expr2);
	self = super_dtor(Arith, _self);
	return (self);
}

static struct s_Expr	*Arith_gen(const void *_self)
{
	const struct s_Arith		*self = _self;

	return (new(Arith, get_op(self), NULL,
			reduce(self->expr1), reduce(self->expr2)));
}

static struct s_Expr	*Arith_reduce(const void *_self)
{
	const struct s_ExprClass	*class = classOf(_self);
	const struct s_Arith		*self = _self;

	return (super_reduce(class, self));
}

/* Return the string representation in Reverse Polish Notation. */
static const char	*Arith_to_string(const void *_self)
{
	const struct s_Arith	*self = _self;
	const char				*str1 = to_string(self->expr1);
	const char				*str2 = token_to_string(get_op(self));
	const char				*str3 = to_string(self->expr2);
	size_t					len1 = strlen(str1);
	size_t					len2 = strlen(str2);
	size_t					len3 = strlen(str3);
	char					*retval;

	retval = calloc(sizeof(char), len1 + len2 + len3 + 3);
	if (retval)
	{
		memcpy(retval, str1, len1);
		retval[len1] = ' ';
		memcpy(retval + len1 + 1, str3, len3);
		retval[len1 + 1 + len3] = ' ';
		memcpy(retval + len1 + 1 + len3 + 1, str2, len2);
	}
	free((char *)str1);
	free((char *)str2);
	free((char *)str3);
	return (retval);
}

/* Evaluate the arithmetic expression. */
static const struct s_Token	*Arith_eval(const void *_self)
{
	// const struct s_ExprClass	*class = classOf(_self);
	const struct s_Arith		*self = _self;
	const struct s_Token		*op = get_op(self);
	const struct s_Token		*expr1 = eval(self->expr1);
	const struct s_Token		*expr2 = eval(self->expr2);
	const struct s_Token		*retval = NULL;

	switch (op->tag)
	{
		case '+':
			retval = numeric_add(expr1, expr2);
			break ;
		case '-':
			retval = numeric_sub(expr1, expr2);
			break ;
		case '*':
			retval = numeric_mul(expr1, expr2);
			break ;
		case '/':
			retval = numeric_div(expr1, expr2);
			break ;
		case '%':
			retval = numeric_mod(expr1, expr2);
			break ;
		case '^':
			retval = numeric_pow(expr1, expr2);
			break ;
	}
	delete((void *)expr1);
	delete((void *)expr2);
	return (retval);
}

void	initArith(void)
{
	initOp();
	if (!Arith)
		Arith = new(ExprClass, "Arith",
				Op, sizeof(struct s_Arith),
				ctor, Arith_ctor,
				dtor, Arith_dtor,
				gen, Arith_gen,
				reduce, Arith_reduce,
				to_string, Arith_to_string,
				eval, Arith_eval,
				0);
}
