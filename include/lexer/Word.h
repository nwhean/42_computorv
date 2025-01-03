#ifndef WORD_H
# define WORD_H

# include "Token.h"

/* all members are public */
struct s_Word	/* Word: Token */
{
	const struct s_Token	_;
	void					*lexeme; /* Str*/
};

extern const struct s_Word	*Word;	/* new(Word, Tag, char *); */
extern const struct s_Word	*Word_plus;
extern const struct s_Word	*Word_minus;
extern const struct s_Word	*Word_mmult;
extern const struct s_Word	*Word_imag;

void	initWord(void);

#endif
