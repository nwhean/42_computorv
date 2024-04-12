#ifndef WORD_H
# define WORD_H

# include "Token.h"

// all members are public
struct s_Word		// Word: Token
{
	const struct s_Token	_;
	char					*lexeme;
};

extern const void	*Word;	// new(Word, tag, lexeme);

void	initWord(void);

#endif
