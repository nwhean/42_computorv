#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Str.h"
#include "UnorderedMap.h"

/* inter */
#include "Arith.h"
#include "Expr.h"
#include "Constant.h"
#include "Id.h"
#include "Node.h"
#include "Op.h"
#include "Unary.h"

/* lexer */
#include "Lexer.h"
#include "Rational.h"
#include "Token.h"
#include "Word.h"

/* parser */
#include "Parser.h"

void	init_container(void)
{
	initStr();
	initUnorderedMap();
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
}

void	init_lexer(void)
{
	initLexer();
	initRational();
	initToken();
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
	program(parser);
	delete(parser);
	delete(lexer);
	return (0);
}
