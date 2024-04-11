#ifndef TOKEN_H
# define TOKEN_H

# include "Object.h"
# include "Object.r"

# define NUM 256
# define ID 257

// all members are public
struct s_Token			// Token: Object
{
	const struct s_Object	_;
	int						tag;
};

extern const void	*Token;			// new(Token, );

void	initToken(void);

#endif
