#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Str.h"
#include "UnorderedMap.h"
#include "Vec.h"

/* inter */
#include "Arith.h"
#include "AssignStmt.h"
#include "BuiltIn.h"
#include "Expr.h"
#include "ExprStmt.h"
#include "FuncDef.h"
#include "Function.h"
#include "Constant.h"
#include "Id.h"
#include "MatExpr.h"
#include "Node.h"
#include "Op.h"
#include "SolveStmt.h"
#include "Stmt.h"
#include "Unary.h"
#include "VecExpr.h"

/* lexer */
#include "Complex.h"
#include "Lexer.h"
#include "Polynomial.h"
#include "Rational.h"
#include "Token.h"
#include "Vector.h"
#include "Word.h"

/* parser */
#include "Parser.h"

/* symbols */
#include "Env.h"

void	init_container(void)
{
	initStr();
	initUnorderedMap();
	initVec();
}

void	init_inter(void)
{
	initArith();
	initAssignStmt();
	initBuiltIn();
	initConstant();
	initExpr();
	initExprStmt();
	initFuncDef();
	initFunction();
	initId();
	initMatExpr();
	initNode();
	initOp();
	initSolveStmt();
	initStmt();
	initUnary();
	initVecExpr();
}

void	init_lexer(void)
{
	initComplex();
	initLexer();
	initPolynomial();
	initRational();
	initToken();
	initVector();
	initWord();
}

void	init_parser(void)
{
	initParser();
}

void	init_symbols(void)
{
	initEnv();
}

/* initialise all classes */
void	init(void)
{
	init_container();
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
	Parser_program(parser);
	delete(parser);
	delete(lexer);
	return (0);
}
