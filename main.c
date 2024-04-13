#include <stdlib.h>
#include <stdio.h>

// inter
#include "Arith.h"
#include "Expr.h"
#include "Constant.h"
#include "Node.h"
#include "Op.h"
#include "Unary.h"

// lexer
#include "Lexer.h"
#include "Num.h"
#include "Real.h"
#include "Token.h"
#include "Word.h"

// parser
#include "Parser.h"

// symbols
#include "Type.h"

void	init_inter(void)
{
	initArith();
	initExpr();
	initConstant();
	initNode();
	initOp();
	initUnary();
}

void	init_lexer(void)
{
	initLexer();
	initNum();
	initReal();
	initToken();
	initWord();
}

void	init_parser(void)
{
	initParser();
}

void	init_symbols(void)
{
	initType();
}

// initialise all classes
void	init(void)
{
	init_inter();
	init_lexer();
	init_parser();
	init_symbols();
}

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
