#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Str.h"
#include "UnorderedMap.h"
#include "Vec.h"

/* inter */
#include "Arith.h"
#include "Expr.h"
#include "Constant.h"
#include "Id.h"
#include "Node.h"
#include "Op.h"
#include "Unary.h"
#include "VecExpr.h"

/* lexer */
#include "Complex.h"
#include "Lexer.h"
#include "Rational.h"
#include "Token.h"
#include "Vector.h"
#include "Word.h"

/* parser */
#include "Parser.h"

void	init_container(void)
{
	initStr();
	initUnorderedMap();
	initVec();
}

void	init_inter(void)
{
	initArith();
	initConstant();
	initExpr();
	initId();
	initNode();
	initOp();
	initUnary();
	initVecExpr();
}

void	init_lexer(void)
{
	initComplex();
	initLexer();
	initRational();
	initToken();
	initVector();
	initWord();
}

void	init_parser(void)
{
	initParser();
}

/* initialise all classes */
void	init(void)
{
	init_container();
	init_inter();
	init_lexer();
	init_parser();
}

#include <string.h>

int	main(void)
{
	void	*lexer;
	void	*parser;

	init();
	lexer = new(Lexer);
	parser = new(Parser, lexer);
	Parser_program(parser);
	delete(parser);
	delete(lexer);
	return (0);
}
