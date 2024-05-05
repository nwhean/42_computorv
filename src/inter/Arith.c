#include <assert.h>
#include <stdarg.h>

/* container */
#include "Str.h"

/* inter */
#include "Arith.h"
#include "Expr.r"
#include "Op.h"

/* lexer */
#include "Numeric.h"
#include "Matrix.h"
#include "Word.h"

const void	*Arith;

/* Arith constructor method. */
static void	*Arith_ctor(void *_self, va_list *app)
{
	struct s_Arith	*self;

	self = super_ctor(Arith, _self, app);
	self->expr1 = va_arg(*app, struct s_Expr *);
	self->expr2 = va_arg(*app, struct s_Expr *);
	set_tag(self, numeric_max(self->expr1->tag, self->expr2->tag));
	/* assert(get_tag(self)); */
	return (self);
}

/* Arith copy constructor method. */
static void	*Arith_copy(const void *_self)
{
	const struct s_Arith	*self = _self;
	struct s_Arith			*retval;

	retval = super_copy(Arith, _self);
	retval->expr1 = copy(self->expr1);
	retval->expr2 = copy(self->expr2);
	return (retval);
}

/* Arith destructor method. */
static void	*Arith_dtor(void *_self)
{
	struct s_Arith	*self = _self;

	delete(self->expr1);
	delete(self->expr2);
	self = super_dtor(Arith, _self);
	return (self);
}

/* Return the string representation in Reverse Polish Notation. */
static char	*Arith_str(const void *_self)
{
	const struct s_Arith	*self = _self;
	const char				*str1 = str(self->expr1);
	const char				*str2 = str(get_op(self));
	const char				*str3 = str(self->expr2);
	void					*s = new(Str, str1);
	char					*retval;

	Str_push_back(s, ' ');
	Str_append(s, str2);
	Str_push_back(s, ' ');
	Str_append(s, str3);
	retval = str(s);
	free((char *)str1);
	free((char *)str2);
	free((char *)str3);
	delete(s);
	return (retval);
}

/* Evaluate the arithmetic expression. */
static struct s_Token	*Arith_eval(const void *_self, void *env)
{
	const struct s_Arith	*self = _self;
	const struct s_Token	*op = get_op(self);
	void					*expr1 = eval(self->expr1, env);
	void					*expr2 = eval(self->expr2, env);
	void					*retval = NULL;

	if (expr1 && expr2)
	{
		switch (op->tag)
		{
			case '+':
				retval = numeric_iadd(&expr1, expr2);
				break ;
			case '-':
				retval = numeric_isub(&expr1, expr2);
				break ;
			case '*':
				retval = numeric_imul(&expr1, expr2);
				break ;
			case MMULT:
				retval = numeric_immult(&expr1, expr2);
				break ;
			case '/':
				retval = numeric_idiv(&expr1, expr2);
				break ;
			case '%':
				retval = numeric_imod(&expr1, expr2);
				break ;
			case '^':
				retval = numeric_ipow(&expr1, expr2);
				break ;
		}
	}
	delete(expr2);
	return (retval);
}

void	initArith(void)
{
	initStr();
	initOp();
	if (!Arith)
		Arith = new(ExprClass, "Arith",
				Op, sizeof(struct s_Arith),
				ctor, Arith_ctor,
				copy, Arith_copy,
				dtor, Arith_dtor,
				str, Arith_str,
				eval, Arith_eval,
				0);
}
