#include <stdlib.h>
#include <stdio.h>

#include "Lexer.h"
#include "Token.h"
#include "Num.h"
#include "Real.h"
#include "Parser.h"

// initialise all classes
void	init(void)
{
	initLexer();
	initToken();
	initNum();
	initReal();
	initParser();
}

int	main(void)
{
	void	*lexer;
	void	*parser;

	init();
	lexer = new(Lexer);
	parser = new(Parser, lexer);
	program(parser);
	printf("\n");
	delete(parser);
	delete(lexer);
	return (0);
}
