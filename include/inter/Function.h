#ifndef FUNCTION_H
# define FUNCTION_H

# include "Expr.h"

/* all members are public */
struct s_Function	/* Function: Expr */
{
	const struct s_Expr	_;
	void				*params;	/* Vec*/
	void				*expr;
};

extern const void	*Function;	/* new(Function, Word, FUNCTION, Vec); */

void	initFunction(void);

#endif
