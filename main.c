#include <stdlib.h>
#include <stdio.h>

#include "Lexer.h"
#include "Token.h"
#include "Num.h"
#include "Parser.h"

int	main(void)
{
	void	*lexer;
	void	*parser;

	initLexer();
	initToken();
	initNum();
	initParser();

	lexer = new(Lexer);
	parser = new(Parser, lexer);
	program(parser);
	printf("\n");
	delete(parser);
	delete(lexer);
	return (0);
}
