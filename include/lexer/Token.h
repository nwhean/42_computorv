#ifndef TOKEN_H
# define TOKEN_H

# include "Object.h"
# include "Object.r"

enum e_Tag {
	NUM = 256,
	REAL,
	ID,
};

// all members are public
struct s_Token			// Token: Object
{
	const struct s_Object	_;
	int						tag;
};

extern const void	*Token;			// new(Token, );

void	initToken(void);

#endif
