#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "str.h"
#include "UnorderedMap.h"
#include "Token.h"
#include "Rational.h"
#include "Word.h"
#include "Lexer.h"
#include "Lexer.r"

/* Lexer constructor method. */
static void	*Lexer_ctor(void *_self, va_list *app)
{
	struct s_Lexer	*self;

	self = super_ctor(Lexer, _self, app);
	self->peek = ' ';
	self->words = new(UnorderedMap, str_equal);
	return (self);
}

/* Lexer destructor method. */
static void	*Lexer_dtor(void *_self)
{
	struct s_Lexer	*self = self;

	self->peek = ' ';
	delete(self->words);
	self = super_dtor(Lexer, _self);
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
		long	val = 0;
		long	d = 1;
		do
		{
			val = 10 * val + self->peek - '0';
			readch(_self);
		} while (isdigit(self->peek));
		if (self->peek != '.')
			return new(Rational, RATIONAL, val, 1);
		while (true)
		{
			readch(_self);
			if (!isdigit(self->peek))
				break ;
			val = 10 * val + self->peek - '0';
			d *= 10;
		}
		return new(Rational, RATIONAL, val, d);
	}

	/* handle words */
	if (isalpha(self->peek))
	{
		char	*str = NULL;
		size_t	len = 0;
		void	*word;

		do
		{
			char	*temp;

			temp = realloc(str, len + 2);
			if (temp == NULL)
			{
				free(str);
				return (NULL);
			}
			str = temp;
			str[len] = self->peek;
			str[len + 1] = '\0';
			len += 1;
			readch(_self);
		} while (isalnum(self->peek));
		word = UnorderedMap_find(self->words, str);
		if (word)
			free(str);
		else
		{
			word = new(Word, ID, str);
			UnorderedMap_insert(self->words, str, word);
		}
		return (word);
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
				dtor, Lexer_dtor,
				scan, Lexer_scan,
				0);
}
