#ifndef VECTOR_H
# define VECTOR_H

# include "Numeric.h"

/* all members are public */
struct s_Vector	/* Vector: Numeric */
{
	const struct s_Numeric	_;
	size_t					size;
	void					**data;
};

extern const void	*Vector;	/* new(Vector, VECTOR, Vec); */

void	initVector(void);

/* capacity */
size_t	Vector_size(const void *self);

/* element access */
void	*Vector_at(const void *self, size_t n);

/* vector operations */
struct s_Vector		*Vector_conjugate(const void *self);
struct s_Vector		*Vector_cross(const void *self, const void *other);
struct s_Rational	*Vector_dot(const void *self, const void *other);
struct s_Rational	*Vector_magnitude(const void *self);
void				*Vector_sum(const void *self);
void				*Vector_sum_product(const void *self, const void *other);

#endif
