#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

extern char	*lookahead;

static void	report(char *s)
{
	fprintf(stderr, "%s\n", s);
	exit(-1);
}

/* <expr>	::=	<term> <rest> */
void	expr(void)
{
	term();
	rest();
}

/*
 * <rest>	::=	+ <term> <rest> | - <term> <rest> | epsilon
 */
void	rest(void)
{
	while (1)
	{
		if (*lookahead == '+')
		{
			match('+');
			term();
			printf("+");
			continue ;
		}
		else if (*lookahead == '-')
		{
			match('-');
			term();
			printf("-");
			continue ;
		}
		else
			return ;
	}
}

/* <term>	:==	0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 */
void	term(void)
{
	if (isdigit(*lookahead))
	{
		printf("%c", *lookahead);
		match(*lookahead);
	}
	else
		report("Syntax Error");
}

void	match(int t)
{
	if (*lookahead == t)
		++lookahead;
	else
		report("Syntax Error");
}
