#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "Lexer.h"
#include "Token.h"
#include "Num.h"
#include "Parser.h"
#include "Parser.r"

static void	move(void *_self);
static void	expr(void *_self);
static void	expr_tail(void *_self);
static void	term(void *_self);
static void	term_tail(void *_self);
static void	factor(void *_self);

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

/* Parser move method */
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
	struct s_Parser	*self = _self;

	expr(self);
	delete(self->look);
}

static void	error(char *s)
{
	fprintf(stderr, "%s\n", s);
	exit(1);
}

static void	match(void *_self, int t)
{
	struct s_Parser	*self = _self;

	if (self->look->tag == t)
		move(self);
	else
		error("match Syntax Error");
}

/* <expr>	::=	<term> <expr_tail> */
static void	expr(void *_self)
{
	term(_self);
	expr_tail(_self);
}

/*
 * <expr_tail>	::=	+ <term> <expr_tail> | - <term> <expr_tail> | epsilon
 */
void	expr_tail(void *_self)
{
	struct s_Parser	*self = _self;

	while (1)
	{
		if (self->look->tag == '+')
		{
			match(_self, '+');
			term(_self);
			printf("+ ");
			continue ;
		}
		else if (self->look->tag == '-')
		{
			match(_self, '-');
			term(_self);
			printf("- ");
			continue ;
		}
		else
			return ;
	}
}

/*
 * <term>	:== <factor> <term_tail>
 */
void	term(void *_self)
{
	factor(_self);
	term_tail(_self);
}

/*
 * <term_tail>	:== * <factor> <term_tail> | / <factor> <term_tail> | epsilon
 */
void	term_tail(void *_self)
{
	struct s_Parser	*self = _self;

	while (1)
	{
		if (self->look->tag == '*')
		{
			match(_self, '*');
			factor(_self);
			printf("* ");
			continue ;
		}
		else if (self->look->tag == '/')
		{
			match(_self, '/');
			factor(_self);
			printf("/ ");
			continue ;
		}
		else
			return ;
	}
}

/* <factor>	:==	(expr) | Num */
void	factor(void *_self)
{
	struct s_Parser	*self = _self;

	if (self->look->tag == '(')
	{
		match(_self, '(');
		expr(_self);
		match(_self, ')');
	}
	else if (self->look->tag == NUM)
	{
		struct s_Num	*look = (struct s_Num *)self->look;

		printf("%d ", look->value);
		match(_self, self->look->tag);
	}
	else
		error("factor Syntax Error");
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
				program, Parser_program,
				0);
}
