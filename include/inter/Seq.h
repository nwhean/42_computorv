#ifndef SEQ_H
# define SEQ_H

# include "Stmt.h"

struct s_Seq	/* Seq: Stmt */
{
	const struct s_Stmt	_;
	struct s_Stmt		*stmt1;
	struct s_Stmt		*stmt2;
};

extern const void	*Seq;	/* new(Seq, Stmt1, Stmt2); */

void	initSeq(void);

#endif
