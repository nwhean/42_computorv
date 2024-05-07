#ifndef BUILTIN_H
# define BUILTIN_H

# include "Expr.h"

/* all members are public */
struct s_BuiltIn	/* BuiltIn: Expr */
{
	const struct s_Expr	_;
	size_t				count;					/* the number of variables */
	void				*(*fptr)(const void *);	/* function pointer */
};

extern const void	*BuiltIn;	/* new(BuiltIn, Word, BUILTIN, size_t, fptr); */
extern const void	*BuiltInFunc;	/* Environment containing Built-in funcs */

void	*BuiltIn_call(const void *self, const void *params);

void	initBuiltIn(void);

#endif
