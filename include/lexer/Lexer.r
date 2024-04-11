#ifndef LEXER_R
# define LEXER_R

# include "Object.r"
# include "Token.h"

struct s_Lexer			// Lexer: Object
{
	const struct s_Object	_;
	char					peek;
};

struct	s_LexerClass	// LexerClass: Class
{
	const struct s_Class	_;
	struct s_Token			*(*scan)(void *self);
};

#endif
