#ifndef MATRIX_H
# define MATRIX_H

# include "Numeric.h"

/* all members are public */
struct s_Matrix	/* Matrix: Numeric */
{
	const struct s_Numeric	_;
	size_t					rows;
	size_t					cols;
	void					**data;
};

extern const void	*Matrix;	/* new(Matrix, MATRIX, Vec); */

void	initMatrix(void);

/* capacity */
size_t	Matrix_rows(const void *self);
size_t	Matrix_cols(const void *self);

/* element access */
void	*Matrix_at(const void *self, size_t m, size_t n);

#endif
