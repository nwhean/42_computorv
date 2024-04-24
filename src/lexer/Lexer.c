#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

/* container */
#include "Str.h"
#include "UnorderedMap.h"

/* lexer */
#include "Complex.h"
#include "Lexer.h"
#include "Lexer.r"
#include "Rational.h"
#include "Token.h"
#include "Word.h"

static void				Lexer_reserve(void *_self, const struct s_Word *word);
static struct s_Word	*Lexer_find(void *_self, const void *str);

/* Lexer constructor method. */
static void	*Lexer_ctor(void *_self, va_list *app)
{
	struct s_Lexer	*self;

	self = super_ctor(Lexer, _self, app);
	self->peek = ' ';
	self->words = new(UnorderedMap);
	Lexer_reserve(self, (struct s_Word *)Word_imag);	/* i as keyword */
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

/* Return a Rational or a Complex token */
static struct s_Token	*generate_numeric(void *_self, long n, long d)
{
	struct s_Lexer	*self = _self;

	if (self->peek == 'i')
	{
		readch(_self);
		return (new(Complex, COMPLEX,
				new(Rational, RATIONAL, 0, 1),
				new(Rational, RATIONAL, n, d)));
	}
	else
		return (new(Rational, RATIONAL, n, d));
}

/* Scan the input in return a Tokon object. */
struct s_Token	*Lexer_scan(void *_self)
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
			return generate_numeric(self, val, d);
		while (true)
		{
			readch(_self);
			if (!isdigit(self->peek))
				break ;
			val = 10 * val + self->peek - '0';
			d *= 10;
		}
		return generate_numeric(self, val, d);
	}

	/* handle words */
	if (isalpha(self->peek))
	{
		void	*s = new(Str, "");
		void	*word;

		do
		{
			Str_push_back(s, self->peek);
			readch(_self);
		} while (isalnum(self->peek));
		word = Lexer_find(self, s);
		if (!word)
		{
			char	*s_ptr = str(s);
			word = new(Word, ID, s_ptr);
			Lexer_reserve(self, word);
			free(s_ptr);
		}
		delete(s);
		return (word);
	}

	/* other tokens */
	token = new(Token, self->peek);
	self->peek = ' ';
	return token;
}

/* Reserve keywords into a hashtable. */
static void	Lexer_reserve(void *_self, const struct s_Word *word)
{
	struct s_Lexer	*self = _self;

	UnorderedMap_insert(
		self->words, copy(word->lexeme), (struct s_Word *)word);
}

/* Retrive a value from a hashtable. */
static struct s_Word	*Lexer_find(void *_self, const void *str)
{
	struct s_Lexer	*self = _self;

	return (UnorderedMap_find((void *)self->words, str));
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
		(void)method;
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
				0);
}
