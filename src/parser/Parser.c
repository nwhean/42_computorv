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
#include "AssignStmt.h"
#include "Constant.h"
#include "FuncDef.h"
#include "Expr.h"
#include "ExprStmt.h"
#include "Id.h"
#include "MatExpr.h"
#include "Seq.h"
#include "Stmt.h"
#include "Unary.h"
#include "VecExpr.h"

/* lexer */
#include "Complex.h"
#include "Function.h"
#include "Lexer.h"
#include "Rational.h"
#include "Token.h"
#include "Word.h"

/* parser */
#include "Parser.h"
#include "Parser.r"

/* symbols */
#include "Env.h"

const void	*Parser;
const void	*ParserClass;

static void				move(void *_self);
static struct s_Stmt	*stmts(void *_self);
static struct s_Stmt	*stmt(void *_self);
static struct s_Expr	*expr(void *_self);
static struct s_Expr	*term(void *_self);
static struct s_Expr	*unary(void *_self);
static struct s_Expr	*factor(void *_self);
static struct s_Expr	*base(void *_self);
static struct s_Expr	*vector(void *_self);
static struct s_Expr	*matrix(void *_self);
static void				*param(void *_self);

/* Parser constructor method. */
static void	*Parser_ctor(void *_self, va_list *app)
{
	struct s_Parser	*self;

	self = super_ctor(Parser, _self, app);
	self->lexer = va_arg(*app, void *);
	self->look = NULL;
	self->top = new(Env, NULL);	/* symbol table */
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
		delete(self->look);
	self->look = Lexer_scan(self->lexer);
}

/* Runs the parser. */
void	Parser_program(void *_self)
{
	struct s_Parser		*self = _self;
	void				*x = stmts(self);	/* Seq */

	if (x)
		exec(x, self->top);
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

/* <stmts>		::= <stmts_tail>
 * <stmts_tail>	::= <stmt> <stmts_tail> | epsilon
 */
static struct s_Stmt	*stmts(void *_self)
{
	struct s_Parser	*self = _self;
	struct s_Stmt	*x;

	if (self->look->tag == EOF)
		return (NULL);
	x = stmt(self);
	return new(Seq, x, stmts(self));
}

/* <stmt>			::= <stmt_type> '\n'
 * <stmt_type>		::= <null_stmt> | <expr_stmt> | <assign_stmt>
 * <null_stmt>		::= epsilon
 * <expr_stmt>		::= <expr> | <expr> '=' '?'
 * <assign_stmt>	::= <assign_lhs> '=' <expr>
 * <assign_lhs>		::= <id> | <func>
 */
static struct s_Stmt	*stmt(void *_self)
{
	struct s_Parser	*self = _self;
	void			*lhs = NULL;	/* Expr */
	void			*rhs = NULL;	/* Expr */
	void			*retval = NULL;
	enum e_Tag		tag;

	if (self->look->tag == '\n')	/* null statement */
	{
		move(self);
		return (NULL);
	}
	lhs = expr(self);
	if (!lhs)
		return (NULL);
	switch (self->look->tag)
	{
		case '\n':
			retval = new(ExprStmt, lhs);
			break ;

		case '=':
			move(self);
			if (self->look->tag == '?')
			{
				move(self);
				retval = new(ExprStmt, lhs);
				break ;
			}

			rhs = expr(self);
			tag = get_tag(lhs);
			if (tag == ID)
				retval = new(AssignStmt, lhs, rhs);
			else
				retval = new(FuncDef, lhs, rhs);
			break ;

		default:
			match(self, '=');
			delete(lhs);
			delete(rhs);
	}
	if (retval)
		match(self, '\n');
	return (retval);
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
	void			*tok;		/* Token or its subclass */
	void			*args;		/* Vec containing Expr */


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
			x = new(Id, copy(self->look), ID);
			move(self);
			return (x);
		case FUNCTION:
			x = copy(self->look);
			move(self);
			args = param(self);
			if (!args)
			{
				delete(x);
				return (NULL);
			}
			return new(Function, x, FUNCTION, args);
		default:
			fprintf(stderr, "base: Unexpected token '%c' (%d)\n",
					self->look->tag, self->look->tag);
			move(self);
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

/* <param_list>	:== <expr> ',' <param_list> | <expr> | epsilon */
static void	*param(void *_self)
{
	struct s_Parser	*self = _self;
	void			*vec = new(Vec);	/* Vec container */
	struct s_Expr	*x;

	match(self, '(');
	if (self->look->tag == ')')
	{
		move(self);
		return (vec);
	}
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
	if (match(self, ')'))
		return (vec);
	delete(vec);
	return (NULL);
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
