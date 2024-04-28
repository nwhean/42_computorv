#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Str.h"
#include "Vec.h"

/* lexer */
#include "Matrix.h"
#include "Rational.h"

/* other */
#include "utility.h"

const void	*Matrix;

static long	max(long a, long b)
{
	return (a > b ? a : b);
}

/* Matrix constructor method. */
static void	*Matrix_ctor(void *_self, va_list *app)
{
	struct s_Matrix	*self;
	void			*vec;
	size_t			i;

	self = super_ctor(Matrix, _self, app);
	vec = va_arg(*app, void *);
	self->rows = Vec_size(vec);
	self->cols = 0;
	for (i = 0; i < self->rows; ++i)
		self->cols = max(self->cols, Vec_size(Vec_at(vec, i)));
	self->data = calloc(sizeof(void *), self->rows * self->cols);
	if (!self->data)
	{
		delete(vec);
		delete(self);
		return (NULL);
	}

	for (i = 0; i < self->rows; ++i)
	{
		void	*vec_i = Vec_at(vec, i);
		void	**data = Vec_data(vec_i);
		size_t	size = Vec_size(vec_i);
		size_t	j;

		for (j = 0; j < size; ++j)
			swap_ptr(&self->data[i * self->cols + j], &data[j]);

		/* handle different column sizes */
		for (; j < self->cols; ++j)
			self->data[i * self->cols + j] = new(Rational, RATIONAL, 0, 1);
	}
	delete(vec);
	return (self);
}

/* Return a copy of the Matrix. */
static struct s_Matrix	*Matrix_copy(const void *_self)
{
	const struct s_Matrix	*self = _self;
	struct s_Matrix			*retval;
	size_t					i;

	retval = super_copy(Matrix, self);
	retval->rows = self->rows;
	retval->cols = self->cols;
	retval->data = calloc(sizeof(void *), self->rows * self->cols);
	if (!retval->data)
	{
		delete(retval);
		return (NULL);
	}
	for (i = 0; i < retval->rows * retval->cols; ++i)
		retval->data[i] = copy(self->data[i]);
	return (retval);
}

/* Matrix destructor method. */
static void	*Matrix_dtor(void *_self)
{
	struct s_Matrix	*self = _self;
	size_t			i;

	for (i = 0; i < self->rows * self->cols; ++i)
		delete(self->data[i]);
	free(self->data);
	return(super_dtor(Matrix, _self));
}

/* Return string representing the Matrix. */
static char	*Matrix_str(const void *_self)
{
	const struct s_Matrix	*self = _self;
	void				*s = new(Str, "");	/* Str */
	char				*s_append;
	char				*retval;
	size_t				i;
	size_t				j;

	for (i = 0; i < self->rows; ++i)
	{
		Str_push_back(s, '[');
		for (j = 0; j < self->cols; ++j)
		{
			s_append = str(Matrix_at(self, i, j));
			Str_append(s, s_append);
			free(s_append);
			if (j < self->cols - 1)
				Str_append(s, ", ");
		}
		Str_push_back(s, ']');
		if (i < self->rows - 1)
			Str_append(s, "\n");
	}
	retval = str(s);
	delete(s);
	return (retval);
}

/* Element-by-element operation between a Matrix and a Scalar.*/
static void	*Matrix_op_Scalar(const void *_self,
							const void *_other,
							void* (*func)(const void *, const void *))
{
	const struct s_Matrix	*self = _self;
	struct s_Matrix			*retval;
	size_t					i;

	retval = super_copy(Matrix, self);
	retval->rows = self->rows;
	retval->cols = self->cols;
	retval->data = calloc(sizeof(void *), self->rows * self->cols);
	if (!retval->data)
	{
		delete(retval);
		return (NULL);
	}
	for (i = 0; i < retval->rows * retval->cols; ++i)
		retval->data[i] = func(self->data[i], _other);
	return (retval);
}

/* Element-by-element operation between two Matrixs.*/
static void	*Matrix_op_Matrix(const void *_self,
							const void *_other,
							void* (*func)(const void *, const void *))
{
	const struct s_Matrix	*self = _self;
	const struct s_Matrix	*other = _other;
	struct s_Matrix			*retval;
	size_t					i;

	if (self->rows != other->rows || self->cols != other->cols)
	{
		fprintf(stderr, "%s\n", "Matrix_op_Matrix: incompatible sizes.");
		return (NULL);
	}
	retval = super_copy(Matrix, self);
	retval->rows = self->rows;
	retval->cols = self->cols;
	retval->data = calloc(sizeof(void *), self->rows * self->cols);
	if (!retval->data)
	{
		delete(retval);
		return (NULL);
	}
	for (i = 0; i < retval->rows * retval->cols; ++i)
		retval->data[i] = func(self->data[i], other->data[i]);
	return (retval);
}

/* Return the addition of two Numerics. */
static void	*Matrix_add(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			fprintf(stderr, "%s\n", "Matrix_add: incompatible with scalar.");
			return (NULL);
		case VECTOR:
			fprintf(stderr, "%s\n", "Matrix_add: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			return (Matrix_op_Matrix(_self, _other, numeric_add));
		default:
			fprintf(stderr, "%s\n", "Matrix_add: unexpected input type.");
			return (NULL);
	};
}

/* Return the subtraction of one Numeric from another. */
static void	*Matrix_sub(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			fprintf(stderr, "%s\n", "Matrix_sub: incompatible with scalar.");
			return (NULL);
		case VECTOR:
			fprintf(stderr, "%s\n", "Matrix_sub: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			return Matrix_op_Matrix(_self, _other, numeric_sub);
		default:
			fprintf(stderr, "%s\n", "Matrix_sub: unexpected input type.");
			return (NULL);
	};
}

/* Return the multiplication of two Numerics. */
static void	*Matrix_mul(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			return Matrix_op_Scalar(_self, _other, numeric_mul);
		case VECTOR:
			fprintf(stderr, "%s\n", "Matrix_mul: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			return Matrix_op_Matrix(_self, _other, numeric_mul);
		default:
			fprintf(stderr, "%s\n", "Matrix_mul: unexpected input type.");
			return (NULL);
	};
}

/* Return the division of one Numeric from another. */
static void	*Matrix_div(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			return Matrix_op_Scalar(_self, _other, numeric_div);
		case VECTOR:
			fprintf(stderr, "%s\n", "Matrix_div: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			return Matrix_op_Matrix(_self, _other, numeric_div);
		default:
			fprintf(stderr, "%s\n", "Matrix_div: unexpected input type.");
			return (NULL);
	};
}

/* Return remainder from the division of one Numeric from another. */
static void	*Matrix_mod(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			return Matrix_op_Scalar(_self, _other, numeric_mod);
		case VECTOR:
			fprintf(stderr, "%s\n", "Matrix_mod: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Matrix_mod: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Matrix_mod: unexpected input type.");
			return (NULL);
	};
}

/* Return a copy of the Matrix with its value negated. */
static struct s_Matrix	*Matrix_neg(const void *_self)
{
	const struct s_Matrix	*self = _self;
	struct s_Rational		*r = new(Rational, RATIONAL, (long)-1, 1);
	struct s_Matrix			*retval = Matrix_mul(self, r);

	delete(r);
	return (retval);
}

/* Return the exponentiation of one Numeric to another. */
static void	*Matrix_pow(const void *_self, const void *_other)
{
	(void)_self;
	(void)_other;
	fprintf(stderr, "%s\n", "Matrix_pow: Undefined.");
	return (NULL);
}

/* Return true if two Matrixs are the same, false otherwise. */
static bool	Matrix_equal(const void *_self, const void *_other)
{
	const struct s_Matrix	*self = _self;
	const struct s_Matrix	*other = _other;
	size_t					i;

	if (!super_equal(Matrix, _self, _other))
		return (false);
	if (self->rows != other->rows)
		return (false);
	if (self->cols != other->cols)
		return (false);
	for (i = 0; i < self->rows * self->cols; ++i)
	if (!equal(self->data[i], other->data[i]))
		return (false);
	return (true);
}

/* Promote one Numeric type to another */
static void	*Matrix_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Matrix	*self = _self;

	switch (tag)
	{
		case RATIONAL:
		case COMPLEX:
			fprintf(stderr, "%s\n",
					"Matrix_promotion: cannot demote Matrix to scalar type.");
			return (NULL);
		case VECTOR:
			fprintf(stderr, "%s\n",
					"Matrix_promotion: cannot demote Matrix to Vector type.");
			return (NULL);
		case MATRIX:
			return Matrix_copy(self);
		default:
			fprintf(stderr, "%s\n","Matrix_promotion: unexpected input type.");
			return (NULL);
	};
}

void	initMatrix(void)
{
	initNumeric();
	if (!Matrix)
	{
		Matrix = new(NumericClass, "Matrix",
				Numeric, sizeof(struct s_Matrix),
				ctor, Matrix_ctor,
				copy, Matrix_copy,
				dtor, Matrix_dtor,
				str, Matrix_str,
				equal, Matrix_equal,
				numeric_add, Matrix_add,
				numeric_sub, Matrix_sub,
				numeric_mul, Matrix_mul,
				numeric_div, Matrix_div,
				numeric_mod, Matrix_mod,
				numeric_neg, Matrix_neg,
				numeric_pow, Matrix_pow,
				numeric_promote, Matrix_promote,
				0);
		initStr();
		initVec();
		initRational();
	}
}


/* Return the row size of the Matrix. */
size_t	Matrix_rows(const void *_self)
{
	const struct s_Matrix	*self = _self;

	return (self->rows);
}

/* Return the column size of the Matrix. */
size_t	Matrix_cols(const void *_self)
{
	const struct s_Matrix	*self = _self;

	return (self->cols);
}

/* Returns a reference to the element at row `m` and column `n`. */
void	*Matrix_at(const void *_self, size_t m, size_t n)
{
	const struct s_Matrix	*self = _self;

	return (self->data[m * self->cols + n]);
}
