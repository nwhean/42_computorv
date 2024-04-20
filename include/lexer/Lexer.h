#ifndef LEXER_H
# define LEXER_H

# include "Token.h"

extern const void	*Lexer;	/* new(Lexer) */

extern const void	*LexerClass;
struct s_Token		*Lexer_scan(void *_self);

void			initLexer(void);

#endif
