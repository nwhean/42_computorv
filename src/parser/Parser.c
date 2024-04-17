#include <assert.h>
#include <ctype.h>
#include <stdio.h>

/* common */
#include "str.h"

/* container */
#include "Container.h"
#include "UnorderedMap.h"

/* inter */
#include "Arith.h"
#include "Constant.h"
#include "Expr.h"
#include "Id.h"
#include "Unary.h"

/* lexer */
#include "Lexer.h"
#include "Rational.h"
#include "Token.h"
#include "Word.h"

/* parser */
#include "Parser.h"
#include "Parser.r"

static void				move(void *_self);
static struct s_Expr	*expr(void *_self);
static struct s_Expr	*term(void *_self);
static struct s_Expr	*unary(void *_self);
static struct s_Expr	*factor(void *_self);
static struct s_Expr	*base(void *_self);

/* Parser constructor method. */
static void	*Parser_ctor(void *_self, va_list *app)
{
	struct s_Parser	*self;

	self = super_ctor(Parser, _self, app);
	self->lexer = va_arg(*app, void *);
	self->look = NULL;
	self->top = new(UnorderedMap, str_equal);
	move(self);
	return (self);
}

/* Parser destructor method. */
static void	*Parser_dtor(void *_self)
{
	struct s_Parser	*self = _self;

	delete(self->look);
	delete(self->top);
	return (super_dtor(Parser, _self));
}

/* Advaces the lookahead symbol to the next input. */
static void	move(void *_self)
{
	struct s_Parser	*self = _self;

	if (self->look)
	{
		if (self->look->tag != ID)
			delete(self->look);
	}
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
	void					*x = expr(self);	/* Expr or subclass */
	const char				*s;
	const struct s_Token	*token;

	if (!x)
		return ;

	s = to_string(x);
	printf("%s\n", s);
	free((char *)s);

	token = eval(x);
	if (token)
	{
		const char	*result = token_to_string(token);

		printf("%s\n", result);
		free((char *)result);
		if (token_get_tag(token) != ID)
			delete((void *)token);
	}
	else
	{
		fprintf(stderr, "Evaluation failure\n");
	}
	delete(x);
}

static void	Parser_error(char *s)
{
	fprintf(stderr, "%s\n", s);
}

/* Compares 't' with the lookahead symbol, and advances to the next input. */
static void	match(void *_self, int t)
{
	struct s_Parser	*self = _self;

	if (self->look->tag == t)
		move(self);
	else
		Parser_error("match: Syntax Error");
}

/* <expr>		::= <term> <expr_tail>
 * <expr_tail>	::= + <term> <expr_tail> | - <term> <expr_tail> | epsilon
 */
static struct s_Expr	*expr(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = term(self);

	if (!x)
		return (NULL);
	while (self->look->tag == '+' || self->look->tag == '-')
	{
		struct s_Expr	*rhs;
		enum e_Tag		tag_bak = self->look->tag;

		move(self);
		rhs = term(self);
		if (!rhs)
		{
			delete(x);
			return (NULL);
		}
		x = new(Arith, new(Token, tag_bak), 0, x, rhs);
	}
	if (numeric_is(self->look->tag))
	{
		Parser_error("term: Syntax Error");
		delete(x);
		return (NULL);
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
	enum e_Tag		tag = self->look->tag;

	if (!x)
		return (NULL);
	while (tag == '*' || tag == '/' || tag == '%')
	{
		struct s_Expr	*rhs;
		enum e_Tag		tag_bak = self->look->tag;

		move(self);
		rhs = unary(self);
		if (!rhs)
		{
			delete(x);
			return (NULL);
		}
		x = new(Arith, new(Token, tag_bak), 0, x, rhs);
		tag = self->look->tag;
	}
	return (x);
}

/* <unary>		:== '+' <unary> | '-' <unary> | <factor> */
static struct s_Expr	*unary(void *_self)
{
	struct s_Parser	*self = _self;
	enum e_Tag		tag = self->look->tag;

	if (tag == '+' || tag == '-')
	{
		struct s_Expr	*rhs;

		move(self);
		rhs = unary(self);
		if (rhs)
			return new(Unary, tag == '+' ? Word_plus: Word_minus, ZERO, rhs);
		return (NULL);
	}
	return (factor(self));
}

/*
 * <factor>			:== <base> <factor_tail> | <base>
 * <factor_tail>	:== '^' <factor>
 */
static struct s_Expr	*factor(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = base(self);

	if (!x)
		return (NULL);
	while (self->look->tag == '^')
	{
		struct s_Expr	*rhs;
		enum e_Tag		tag_bak = self->look->tag;

		move(self);
		rhs = factor(self);
		if (!rhs)
		{
			delete(x);
			return (NULL);
		}
		x = new(Arith, new(Token, tag_bak), 0, x, rhs);
	}
	return (x);
}

/* <base>	:== '(' <expr> ')' | Rational | Id */
static struct s_Expr	*base(void *_self)
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
		case RATIONAL:
			tok = token_copy((struct s_Rational *)self->look);
			x = new(Constant, tok, self->look->tag);
			move(self);
			return (x);
		case ID:
			tok = container_find(self->top, (struct s_Word *)self->look);
			if (!tok)
			{
				struct s_Pair	pair;

				tok = token_copy(self->look);
				pair.first = strdup(((struct s_Word *)tok)->lexeme);
				pair.second = tok;
				container_insert(self->top, &pair);
			}
			x = new(Id, token_copy(tok), self->look->tag);
			move(self);
			return (x);
		default:
			Parser_error("base: Syntax Error");
	}
	return (x);
}

/* ParserClass constructor method. */
static void	*ParserClass_ctor(void *_self, va_list *app)
{
	typedef void			(*voidf)();
	struct s_ParserClass	*self;
	voidf					selector;
	va_list					ap;

	self = super_ctor(ParserClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
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
