#include <ctype.h>

#include "Token.h"
#include "Num.h"

extern const char	*lookahead;

void	*scan(void)
{
	// skip whitespace
	while (*lookahead == ' ')
		++lookahead;

	// handle numbers
	if (isdigit(*lookahead))
	{
		int	val = 0;
		do
		{
			val = 10 * val + *lookahead++ - '0';
		} while (isdigit(*lookahead));
		return new(Num, NUM, val);
	}

	// other tokens
	return new(Token, *lookahead++);
}
