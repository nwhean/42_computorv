#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

char	*lookahead = NULL;

int	main(int argc, char **args)
{
	if (argc == 1)
		return (0);
	lookahead = *++args;
	expr();
	printf("\n");
	return (0);
}
