#ifndef PARSER_R
# define PARSER_R

# include "Object.r"
# include "Token.h"

struct s_Parser			// Parser: Object
{
	const struct s_Object	_;
	void					*lexer;	// lexical analyzer for this parser
	struct s_Token			*look;	// lookahead token
};

struct	s_ParserClass	// ParserClass: Class
{
	const struct s_Class	_;
	void				(*program)(void *self);
};

#endif
