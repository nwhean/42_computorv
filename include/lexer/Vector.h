#ifndef VECTOR_H
# define VECTOR_H

# include "Numeric.h"

/* all members are public */
struct s_Vector	/* Vector: Numeric */
{
	const struct s_Numeric	_;
	void					*vec;
};

extern const void	*Vector;	/* new(Vector, VECTOR, Vec); */

void	initVector(void);

/* capacity */
size_t	Vector_size(const void *self);

/* element access */
void	*Vector_at(const void *self, size_t n);

#endif
