#ifndef LEXER_R
# define LEXER_R

# include "Object.r"
# include "Token.h"
# include "Word.h"

struct s_Lexer	/* Lexer: Object */
{
	const struct s_Object	_;
	char					peek;
	void					*words;
};

struct	s_LexerClass	/* LexerClass: Class */
{
	const struct s_Class	_;
	struct s_Token			*(*scan)(void *self);
};

void	Lexer_reserve(void *_self, struct s_Word *word);

#endif
