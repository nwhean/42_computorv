#ifndef PARSER_H
# define PARSER_H

extern const void	*Parser;	/* new(Parser, lexer); */

extern const void	*ParserClass;
void	Parser_program(void *self);

void	initParser(void);

#endif
