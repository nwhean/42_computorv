#ifndef WORD_H
# define WORD_H

# include "Token.h"
# include "Token.r"

/* all members are public */
struct s_Word	/* Word: Token */
{
	const struct s_Token	_;
	void					*lexeme; /* Str*/
};

extern const struct s_Word	*Word;	/* new(Word, Tag, char *); */
extern const struct s_Word	*Word_plus;
extern const struct s_Word	*Word_minus;

void	initWord(void);

#endif
