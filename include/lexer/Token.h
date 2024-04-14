#ifndef TOKEN_H
# define TOKEN_H

# include "Object.h"
# include "Object.r"

enum e_Tag {
	BASIC = 256,
	PLUS,
	MINUS,
	NUM,
	REAL,
	WORD,
	ID,
};

// all members are public
struct s_Token			// Token: Object
{
	const struct s_Object	_;
	int						tag;
};

extern const void	*Token;			// new(Token, tag);

struct	s_TokenClass	// TokenClass: Class
{
	const struct s_Class	_;
	struct s_Token			*(*copy)(const void *self);
	const char				*(*to_string)(const void *self);
};

extern const void	*TokenClass;
void		*token_copy(const void *self);
const char	*token_to_string(const void *self);

void		initToken(void);

#endif
