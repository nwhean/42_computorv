#ifndef VECTOR_H
# define VECTOR_H

# include "Numeric.h"

/* all members are public */
struct s_Vector	/* Vector: Numeric */
{
	const struct s_Numeric	_;
	size_t					size;
	void					**nums;
};

extern const void	*Vector;	/* new(Vector, VECTOR, size_t, void *, ...); */

void	initVector(void);

#endif
