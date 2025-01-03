#ifndef TOKEN_H
# define TOKEN_H

# include "Object.h"
# include "Object.r"

enum e_Tag {
	PLUS = 256,	/* unary '+' */
	MINUS,		/* unary '-' */
	MMULT,		/* matrix multiplication "**" */
	IMAG,		/* i */
	ZERO,		/* an empty tag */
	RATIONAL,	/* Rational number */
	COMPLEX,	/* Complex number */
	VECTOR,		/* Vector of Complex or Rational numbers */
	MATRIX,		/* Matrix */
	POLYNOMIAL,	/* Polynomial */
	ID,			/* Identifier */
	FUNCTION,	/* Function */
	BUILTIN		/* Built-in functions*/
};

/* all members are public */
struct s_Token	/* Token: Object */
{
	const struct s_Object	_;
	int						tag;
};

extern const void	*Token;	/* new(Token, tag); */

struct	s_TokenClass	/* TokenClass: Class */
{
	const struct s_Class	_;
};

extern const void	*TokenClass;

void		initToken(void);
enum e_Tag	Token_get_tag(const void *);

#endif
