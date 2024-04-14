#ifndef NUMERIC_H
# define NUMERIC_H

# include "Token.h"
# include "Token.r"

// all members are public
struct s_Numeric			// Numeric: Token
{
	const struct s_Token	_;
};

extern const void	*Numeric;			// new(Numeric, NULL);

struct	s_NumericClass	// NumericClass: TokenClass
{
	const struct s_TokenClass	_;
	struct s_Numeric			*(*add)(const void *self, const void *other);
	struct s_Numeric			*(*unary)(const void *self);
};

extern const void	*NumericClass;
void				*numeric_add(const void *self, const void *other);
void				*numeric_unary(const void *self);

void		initNumeric(void);

#endif
