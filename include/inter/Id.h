#ifndef ID_H
# define ID_H

# include "Expr.h"

struct s_Id	/* Id: Expr */
{
	const struct s_Expr	_;
};

extern const void	*Id;	/* new(Id, Word, Tag); */

void	initId(void);

#endif
