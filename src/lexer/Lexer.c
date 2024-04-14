#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "Token.h"
#include "Num.h"
#include "Real.h"
#include "Lexer.h"
#include "Lexer.r"

/* Lexer constructor method. */
static void	*Lexer_ctor(void *_self, va_list *app)
{
	struct s_Lexer	*self;

	self = super_ctor(Lexer, _self, app);
	self->peek = ' ';
	return (self);
}

/*
 * Read the next input character into variable 'peek'.
 */
static void	readch(void *_self)
{
	struct s_Lexer	*self = _self;

	self->peek = getchar();
}

struct s_Token	*scan(void *self)
{
	const struct s_LexerClass *const	*cp = self;

	assert(self && *cp && (*cp)->scan);
	return ((*cp)->scan(self));
}

struct s_Token	*super_scan(const void *_class, void *_self)
{
	const struct s_LexerClass	*superclass = super(_class);

	assert(_self && superclass->scan);
	return (superclass->scan(_self));
}

/* Lexer scan method */
static struct s_Token	*Lexer_scan(void *_self)
{
	struct s_Lexer	*self = _self;
	struct s_Token	*token;

	for (; ; readch(_self))
	{
		if (self->peek == ' ')
			continue ;
		else
			break ;
	}

	/* handle numbers */
	if (isdigit(self->peek))
	{
		double	x;
		double	d;
		int		val = 0;
		do
		{
			val = 10 * val + self->peek - '0';
			readch(_self);
		} while (isdigit(self->peek));
		if (self->peek != '.')
			return new(Num, NUM, val);
		x = val;
		d = 10;
		while (true)
		{
			readch(_self);
			if (!isdigit(self->peek))
				break ;
			x += (self->peek - '0') / d;
			d *= 10;
		}
		return new(Real, REAL, x);
	}

	/* other tokens */
	token = new(Token, self->peek);
	self->peek = ' ';
	return token;
}

/* LexerClass constructor method. */
static void	*LexerClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_LexerClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(LexerClass, _self, app);
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
		if (selector == (voidf)scan)
			*(voidf *)&self->scan = method;
		#pragma GCC diagnostic pop
	}
	return (self);
}

const void	*Lexer;
const void	*LexerClass;

void	initLexer(void)
{
	if (!LexerClass)
		LexerClass = new(Class, "LexerClass",
				Class, sizeof(struct s_LexerClass),
				ctor, LexerClass_ctor,
				0);
	if (!Lexer)
		Lexer = new(LexerClass, "Lexer",
				Object, sizeof(struct s_Lexer),
				ctor, Lexer_ctor,
				scan, Lexer_scan,
				0);
}
