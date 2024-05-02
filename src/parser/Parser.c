#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* common */
#include "Str.h"

/* container */
#include "UnorderedMap.h"
#include "Vec.h"

/* inter */
#include "Arith.h"
#include "Constant.h"
#include "Expr.h"
#include "Id.h"
#include "MatExpr.h"
#include "Unary.h"
#include "VecExpr.h"

/* lexer */
#include "Complex.h"
#include "Lexer.h"
#include "Rational.h"
#include "Token.h"
#include "Word.h"

/* parser */
#include "Parser.h"
#include "Parser.r"

const void	*Parser;
const void	*ParserClass;

static void				symbol_add(
							struct s_Parser *self,
							const struct s_Word *word);
static struct s_Word	*symbol_find(
							const struct s_Parser *self,
							const void *str);
static void				move(void *_self);
static struct s_Expr	*stmt(void *_self);
static struct s_Expr	*expr(void *_self);
static struct s_Expr	*term(void *_self);
static struct s_Expr	*unary(void *_self);
static struct s_Expr	*factor(void *_self);
static struct s_Expr	*base(void *_self);
static struct s_Expr	*vector(void *_self);
static struct s_Expr	*matrix(void *_self);

/* Parser constructor method. */
static void	*Parser_ctor(void *_self, va_list *app)
{
	struct s_Parser	*self;

	self = super_ctor(Parser, _self, app);
	self->lexer = va_arg(*app, void *);
	self->look = NULL;
	self->top = new(UnorderedMap);	/* symbol table */
	move(self);
	return (self);
}

/* Parser destructor method. */
static void	*Parser_dtor(void *_self)
{
	struct s_Parser	*self = _self;
	enum e_Tag	tag = self->look->tag;

	if (!(tag == ID || tag == IMAG))
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
		enum e_Tag	tag = self->look->tag;
		if (!(tag == ID || tag == IMAG))
			delete(self->look);
	}
	self->look = Lexer_scan(self->lexer);
}

/* Runs the parser. */
void	Parser_program(void *_self)
{
	struct s_Parser		*self = _self;
	void				*x = stmt(self);	/* Expr or subclass */
	const char			*s;
	struct s_Token		*token;

	if (!x)
		return ;

	s = str(x);
	printf("%s\n", s);
	free((char *)s);

	token = eval(x);
	if (token)
	{
		const char	*result = str(token);

		printf("%s\n", result);
		free((char *)result);
		if (token->tag != ID)
			delete(token);
	}
	else
	{
		fprintf(stderr, "Evaluation failure\n");
	}
	delete(x);
}

/* Compares 't' with the lookahead symbol, and advances to the next input. */
static bool	match(void *_self, int t)
{
	struct s_Parser	*self = _self;

	if (self->look->tag == t)
	{
		move(self);
		return (true);
	}
	fprintf(stderr, "match: Syntax Error: expect '%c' (%d), get '%c' (%d)\n",
			t, t, self->look->tag, self->look->tag);
	return (false);
}

/* <stmt>	::= <expr> | epsilon */
static struct s_Expr	*stmt(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = expr(self);

	if (self->look->tag == '\n')
		return (x);
	fprintf(stderr, "match: Syntax Error: expect '\\n' (%d), get '%c' (%d)\n",
			'\n', self->look->tag, self->look->tag);
	delete(x);
	return (NULL);

}

/* <expr>		::= <term> <expr_tail>
 * <expr_tail>	::= <add_op> <term> <expr_tail>
 * <add_op>		::= '+' | '-'
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
		x = new(Arith, new(Token, tag_bak), ZERO, x, rhs);
	}
	return (x);
}

/* <term>		::= <unary> <term_tail>
 * <term_tail>	::= <mult_op> <unary> <term_tail>
 * <mult_op>	::= '*' | '**' | '/' | '%'
 */
static struct s_Expr	*term(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = unary(self);
	enum e_Tag		tag = self->look->tag;

	if (!x)
		return (NULL);
	while (tag == '*' || tag == MMULT || tag == '/' || tag == '%')
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
		if (tag_bak == MMULT)
			x = new(Arith, Word_mmult, ZERO, x, rhs);
		else
			x = new(Arith, new(Token, tag_bak), ZERO, x, rhs);
		tag = self->look->tag;
	}
	return (x);
}

/* <unary>		::= <unary_ops> <unary> | <factor>
 * <unary_ops>	::= '+' | '-'
 */
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

/* <factor>			::= <base> <factor_tail> | <base>
 * <factor_tail>	::= '^' <exponent>
 * <exponent>		::= <unary>
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
		rhs = unary(self);
		if (!rhs)
		{
			delete(x);
			return (NULL);
		}
		x = new(Arith, new(Token, tag_bak), ZERO, x, rhs);
	}
	return (x);
}

/* <base>	::= '(' <expr> ')'
 *				| <rational> | <complex>
 *				| <vector> | <matrix>
 *				| <id>
 */
static struct s_Expr	*base(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Expr	*x = NULL;
	void			*tok;	/* Token or its subclass */
	void			*s;	/* Str */

	switch (self->look->tag)
	{
		case '(':
			move(self);
			x = expr(self);
			if (x && match(self, ')'))
				return (x);
			delete(x);
			return (NULL);
		case '[':
			move(self);
			if (self->look->tag != '[')
				x = vector(self);
			else
				x = matrix(self);
			if (x && match(self, ']'))
				return (x);
			delete(x);
			return (NULL);
		case RATIONAL:
		case COMPLEX:
			tok = copy(self->look);
			x = new(Constant, tok, self->look->tag);
			move(self);
			return (x);
		case IMAG:
			tok = new(Complex, COMPLEX,
					Rational_from_double(0),
					Rational_from_double(1));
			x = new(Constant, tok, COMPLEX);
			move(self);
			return (x);
		case ID:
			s = ((struct s_Word *)self->look)->lexeme;
			tok = symbol_find(self, s);
			/* if symbol is not found, add it to the symbol table. */
			if (!tok)
			{
				tok = copy(self->look);
				symbol_add(self, tok);
			}
			x = new(Id, copy(tok), self->look->tag);
			move(self);
			return (x);
		default:
			fprintf(stderr, "base: Unexpected token '%c' (%d)\n",
					self->look->tag, self->look->tag);
			return (NULL);
	}
}

/* <vector>			::= '[' <vector_tail> ']'
 * <vector_tail>	::= <expr> ',' <vector_tail> | <expr>
 */
static struct s_Expr	*vector(void *_self)
{
	struct s_Parser	*self = _self;
	void			*vec = new(Vec);	/* Vec container */
	struct s_Expr	*x;

	do {
		if (self->look->tag == ',')
			move(self);
		x = expr(self);
		if (!x)
		{
			delete(vec);
			return (NULL);
		}
		Vec_push_back(vec, x);
	} while (self->look->tag == ',');
	x = new(VecExpr, NULL, VECTOR, vec);

	return (x);
}

/* <matrix>			::= '[' <matrix_tail> ']'
 * <matrix_tail>	::= <vector> ';' <matrix_tail> | <vector>
 */
static struct s_Expr	*matrix(void *_self)
{
	struct s_Parser	*self = _self;
	void			*vec = new(Vec);	/* Vec container */
	struct s_Expr	*x = NULL;

	do {
		if (self->look->tag == ';')
			move(self);
		if (!(match(self, '[')		/* opening bracket of vector */
			&& (x = vector(self))
			&& match(self, ']')))	/* closing bracket of a vector */
		{
			delete(vec);
			delete(x);
			return (NULL);
		}
		Vec_push_back(vec, x);
	} while (self->look->tag == ';');
	x = new(MatExpr, NULL, MATRIX, vec);

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

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		(void)method;
		#pragma GCC diagnostic pop
	}
	return (self);
}

/* Add a Word to the symbol table. */
static void	symbol_add(struct s_Parser *self, const struct s_Word *word)
{
	UnorderedMap_insert(self->top, copy(word->lexeme), (void *)word);
}

/* Find a Word from the symbol table. */
static struct s_Word	*symbol_find(
		const struct s_Parser *self, const void *str)
{
	return (UnorderedMap_find(self->top, str));
}

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
				0);
}
