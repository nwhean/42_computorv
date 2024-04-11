#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "Token.h"
#include "Num.h"
#include "Parser.h"

char	*lookahead = NULL;

int	main(int argc, char **args)
{
	void	*parser;

	initToken();
	initNum();
	initParser();

	if (argc == 1)
		return (0);
	lookahead = *++args;
	parser = new(Parser, NULL);
	program(parser);
	printf("\n");
	delete(parser);
	return (0);
}
