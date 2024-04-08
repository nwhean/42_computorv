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

/* <expr>	::=	<term> <expr_tail> */
void	expr(void)
{
	term();
	expr_tail();
}

/*
 * <expr_tail>	::=	+ <term> <expr_tail> | - <term> <expr_tail> | epsilon
 */
void	expr_tail(void)
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

/*
 * <term>	:== <factor> <term_tail>
 */
void	term(void)
{
	factor();
	term_tail();
}

/*
 * <term_tail>	:== * <factor> <term_tail> | / <factor> <term_tail> | epsilon
 */
void	term_tail()
{
	while (1)
	{
		if (*lookahead == '*')
		{
			match('*');
			factor();
			printf("*");
			continue ;
		}
		else if (*lookahead == '/')
		{
			match('/');
			factor();
			printf("/");
			continue ;
		}
		else
			return ;
	}
}

/* <factor>	:==	(expr) | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 */
void	factor(void)
{
	if (*lookahead == '(')
	{
		match('(');
		expr();
		match(')');
	}
	else if (isdigit(*lookahead))
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
