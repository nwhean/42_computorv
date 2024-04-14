#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// inter
#include "Arith.h"
#include "Constant.h"
#include "Expr.h"
#include "Unary.h"

// lexer
#include "Lexer.h"
#include "Num.h"
#include "Real.h"
#include "Token.h"
#include "Word.h"

// parser
#include "Parser.h"
#include "Parser.r"

// symbols
#include "Type.h"

static void				move(void *_self);
static struct s_Expr	*expr(void *_self);
static struct s_Expr	*term(void *_self);
static struct s_Expr	*factor(void *_self);
static struct s_Expr	*unary(void *_self);

/* Parser constructor method. */
static void	*Parser_ctor(void *_self, va_list *app)
{
	struct s_Parser	*self;

	self = super_ctor(Parser, _self, app);
	self->lexer = va_arg(*app, void *);
	self->look = NULL;
	move(self);
	return (self);
}

/* Parser destructor method. */
static void	*Parser_dtor(void *_self)
{
	struct s_Parser	*self = _self;

	delete(self->look);
	return (super_dtor(Parser, _self));
}

/* Advaces the lookahead symbol to the next input. */
static void	move(void *_self)
{
	struct s_Parser	*self = _self;

	delete(self->look);
	self->look = scan(self->lexer);
}

void	program(void *self)
{
	const struct s_ParserClass *const	*cp = self;

	assert(self && *cp && (*cp)->program);
	(*cp)->program(self);
}

void	super_program(const void *_class, void *_self)
{
	const struct s_ParserClass	*superclass = super(_class);

	assert(_self && superclass->program);
	superclass->program(_self);
}

/* Parser program method */
static void	Parser_program(void *_self)
{
	struct s_Parser			*self = _self;
	void					*x = expr(self);
	const char				*s = to_string(x);
	const struct s_Token	*token = eval(x);
	const char				*result = token_to_string(token);

	printf("%s\n", s);
	printf("%s\n", result);
	free((char *)s);
	delete((void *)token);
	free((char *)result);
	delete(x);
}

static void	error(char *s)
{
	fprintf(stderr, "%s\n", s);
	exit(1);
}

/* Compares 't' with the lookahead symbol, and advances to the next input. */
static void	match(void *_self, int t)
{
	struct s_Parser	*self = _self;

	if (self->look->tag == t)
		move(self);
	else
		error("match Syntax Error");
}

/* <expr>		::= <term> <expr_tail>
 * <expr_tail>	::= + <term> <expr_tail> | - <term> <expr_tail> | epsilon
 */
static struct s_Expr	*expr(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = term(self);

	while (self->look->tag == '+' || self->look->tag == '-')
	{
		struct s_Token	*tok = new(Token, self->look->tag);
		move(self);
		x = new(Arith, tok, NULL, x, term(self));
	}
	return (x);
}

/*
 * <term>		:== <unary> <term_tail>
 * <term_tail>	:== '*' <unary> <term_tail> | '/' <unary> <term_tail> | epsilon
 */
static struct s_Expr	*term(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = unary(self);
	int				tag = self->look->tag;

	while (tag == '*' || tag == '/' || tag == '%')
	{
		struct s_Token	*tok = new(Token, self->look->tag);
		move(self);
		x = new(Arith, tok, NULL, x, unary(self));
		tag = self->look->tag;
	}
	return (x);
}

/* <unary>		:== '+' <unary> | '-' <unary> | <factor> */
static struct s_Expr	*unary(void *_self)
{
	struct s_Parser	*self = _self;

	if (self->look->tag == '+')
	{
		move(self);
		return new(Unary, Word_plus, NULL, unary(self));
	}
	else if (self->look->tag == '-')
	{
		move(self);
		return new(Unary, Word_minus, NULL, unary(self));
	}
	else
		return (factor(self));
}

/* <factor>	:== '(' <expr> ')' | Num | Real */
static struct s_Expr	*factor(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = NULL;
	struct s_Token	*tok;

	switch (self->look->tag)
	{
		case '(':
			move(self);
			x = expr(self);
			match(self, ')');
			return (x);
		case NUM:
			tok = new(Num, NUM, ((struct s_Num *)self->look)->value);
			x = new(Constant, tok, Type_Int);
			move(self);
			return (x);
		case REAL:
			tok = new(Real, REAL, ((struct s_Real *)self->look)->value);
			x = new(Constant, tok, Type_Real);
			move(self);
			return (x);
		default:
			error("factor Syntax Error");
			return (x);
	}
}

/* ParserClass constructor method. */
static void	*ParserClass_ctor(void *_self, va_list *app)
{
	typedef void			(*voidf)();
	struct s_ParserClass	*self;
	voidf					selector;
	va_list					ap;

	self = super_ctor(ParserClass, _self, app);
	va_copy(ap, *app);
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		method = va_arg(ap, voidf);
		if (selector == (voidf)program)
			*(voidf *)&self->program = method;
	}
	return (self);
}

const void	*Parser;
const void	*ParserClass;

void	initParser(void)
{
	if (!ParserClass)
		ParserClass = new(Class, "ParserClass",
				Class, sizeof(struct s_ParserClass),
				ctor, ParserClass_ctor,
				0);
	if (!Parser)
		Parser = new(ParserClass, "Parser",
				Object, sizeof(struct s_Parser),
				ctor, Parser_ctor,
				dtor, Parser_dtor,
				program, Parser_program,
				0);
}
