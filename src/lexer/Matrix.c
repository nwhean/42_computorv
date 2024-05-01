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
#include "Vector.h"

/* other */
#include "mathematics.h"
#include "utility.h"

const void	*Matrix;

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

/* initialise the data structure for Matrix */
static struct s_Matrix	*Matrix_init(const struct s_Matrix *self,
									size_t rows, size_t cols)
{
	struct s_Matrix	*retval = super_copy(Matrix, self);

	retval->rows = rows;
	retval->cols = cols;
	retval->data = calloc(sizeof(void *), rows * cols);
	if (!retval->data)
	{
		delete(retval);
		return (NULL);
	}
	return (retval);
}

/* Return a copy of the Matrix. */
static struct s_Matrix	*Matrix_copy(const void *_self)
{
	const struct s_Matrix	*self = _self;
	struct s_Matrix			*retval;
	size_t					i;

	retval = Matrix_init(self, self->rows, self->cols);
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
	struct s_Matrix			*retval = Matrix_init(self, self->rows, self->cols);
	size_t					i;

	if (!retval)
		return (NULL);
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
	retval = Matrix_init(self, self->rows, self->cols);
	if (!retval)
		return (NULL);
	for (i = 0; i < retval->rows * retval->cols; ++i)
		retval->data[i] = func(self->data[i], other->data[i]);
	return (retval);
}

/* Return the addition of two Numerics. */
static void	*Matrix_add(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
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
	switch (Token_get_tag(_other))
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
	switch (Token_get_tag(_other))
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

/* Return the matrix multiplication of two Matrices. */
void	*Matrix_mmult(const void *_self, const void *_other)
{
	const struct s_Matrix	*self = _self;
	const struct s_Matrix	*other = _other;
	struct s_Matrix			*retval = NULL;
	size_t					i;
	size_t					j;
	size_t					k;

	if (Token_get_tag(_other) != MATRIX)
	{
		fprintf(stderr, "%s\n", "Matrix_mmult: only Matrix input allowed.");
		return (NULL);
	}
	if (self->cols != other->rows)
	{
		fprintf(stderr, "%s\n", "Matrix_mmult: Incompatible matrix sizes.");
		return (NULL);
	}
	retval = Matrix_init(self, self->rows, other->cols);
	if (!retval)
		return (NULL);
	for (i = 0; i < retval->rows * retval->cols; ++i)
		retval->data[i] = new(Rational, RATIONAL, 0, 1);
	for (i = 0; i < self->rows; ++i)
	{
		for (k = 0; k < self->cols; ++k)
		{
			for (j = 0; j < other->cols; ++j)
			{
				void	*mul= numeric_mul(self->data[i * self->cols + k],
										other->data[k * other->cols + j]);
				numeric_iadd(&retval->data[i * other->cols + j], mul);
				delete(mul);
			}
		}
	}
	return (retval);
}

/* Return the division of one Numeric from another. */
static void	*Matrix_div(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
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
	switch (Token_get_tag(_other))
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

static void	*Matrix_pow_Rational(const void *_self, const void *_other)
{
	const struct s_Matrix	*self = _self;
	const struct s_Rational	*other = _other;
	struct s_Matrix			*retval = NULL;

	if (self->rows != self->cols)
	{
		fprintf(stderr, "%s\n", "Matrix_pow_Rational: matrix is not square.");
		return (NULL);
	}
	if (other->denominator != 1)
	{
		fprintf(stderr, "%s\n",
				"Matrix_pow_Rational: only support integer exponent.");
		return (NULL);
	}
	if (other->numerator < 0)
	{
		fprintf(stderr, "%s\n",
				"Matrix_pow_Rational: only support positive integer exponent.");
		return (NULL);
	}
	if (other->numerator == 0)
		return (Matrix_eye(self->rows));
	else
	{
		struct s_Matrix	*temp;
		long			exponent = other->numerator;

		retval = copy(self);
		while (exponent-- > 1)
		{
			temp = retval;
			retval = Matrix_mmult(temp, self);
			delete(temp);
		}
		return (retval);
	}
}

/* Return the exponentiation of one Numeric to another. */
static void	*Matrix_pow(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			return Matrix_pow_Rational(_self, _other);
		case COMPLEX:
			fprintf(stderr, "%s\n", "Matrix_pow: incompatible with Complex.");
			return (NULL);
		case VECTOR:
			fprintf(stderr, "%s\n", "Matrix_pow: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Matrix_pow: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Matrix_pow: unexpected input type.");
			return (NULL);
	};
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
				numeric_mmult, Matrix_mmult,
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

/* Return the size n identity matrix. */
void	*Matrix_eye(size_t n)
{
	struct s_Matrix	*template;
	struct s_Matrix	*retval;
	size_t			i;
	size_t			j;

	if (n == 0)
	{
		fprintf(stderr, "%s\n", "Matrix_eye: n cannot be 0.");
		return (NULL);
	}
	template = new(Matrix, MATRIX, new(Vec));
	retval = Matrix_init(template, n, n);
	if (retval)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				retval->data[i * n + j]
					= new(Rational, RATIONAL, i == j ? 1: 0, 1);
			}
		}
	}
	delete(template);
	return (retval);
}

/* Return the transpose of a Matrix. */
void	*Matrix_transpose(const void *_self)
{
	const struct s_Matrix	*self = _self;
	struct s_Matrix			*retval = Matrix_init(self, self->cols, self->rows);
	size_t					i;
	size_t					j;

	for (i = 0; i < retval->rows; ++i)
	{
		for (j = 0; j < retval->cols; ++j)
		{
			retval->data[i * retval->cols + j]
				= copy(self->data[j * self->cols + i]);
		}
	}
	return (retval);
}

/* swap the row `i` and row `j` of matrix `A`.*/
static void	swap_row(struct s_Matrix *A, size_t i, size_t j)
{
	size_t	k;

	for (k = 0; k < A->cols; ++k)
		swap_ptr(&A->data[i * A->cols + k], &A->data[j * A->cols + k]);
}

/* INPUT:
 * `A` - array of pointers to rows of a square matrix having dimension N
 * `Tol` - small tolerance number to detect failure when the matrix is
 *		 near degenerate
 * OUTPUT:
 * Matrix `A` is changed, it contains a copy of both matrices L-E and U
 * as A=(L-E)+U such that P*A=L*U. The permutation matrix is not stored
 * as a matrix, but in an integer vector P of size N+1 containing column
 * indexes where the permutation matrix has "1". The last element P[N] = S+N,
 * where S is the number of row exchanges needed for determinant computation, det(P)=(-1)^S
 *
 * Reference: https://en.wikipedia.org/wiki/LU_decomposition
 */
static bool	LUP_decompose(struct s_Matrix *A, size_t *P)
{
	size_t				i;
	size_t				j;
	size_t				k;
	size_t				imax;
	size_t				N = A->cols;
	struct s_Rational	*maxA = NULL;
	struct s_Rational	*absA = NULL;
	struct s_Rational	*Tol = new(Rational, RATIONAL, 1, (long)1e8);

	/* Unit permutation matrix, P[N] initialized with N */
	for (i = 0; i <= N; ++i)
		P[i] = i;

	for (i = 0; i < N; i++)
	{
		delete(maxA);
		maxA = new(Rational, RATIONAL, 0, 1);
		imax = i;

		for (k = i; k < N; k++)
		{
			delete(absA);
			absA = copy(Matrix_at(A, k, i));
			if (absA->numerator < 0)
				absA->numerator *= -1;
			if (Rational_gt(absA, maxA))
			{
				delete(maxA);
				maxA = copy(absA);
				imax = k;
			}
		}

		/* failure, matrix is degenerate */
		if (Rational_lt(maxA, Tol))
		{
			delete(absA);
			delete(maxA);
			delete(Tol);
			return (false);
		}

		if (imax != i)
		{
			/* pivoting P */
			j = P[i];
			P[i] = P[imax];
			P[imax] = j;

			/* pivoting rows of A */
			swap_row(A, i, imax);

			/* counting pivots starting from N (for determinant) */
			P[N]++;
		}

		for (j = i + 1; j < N; j++)
		{
			numeric_idiv(&A->data[j * A->cols + i],  A->data[i * A->cols + i]);

			for (k = i + 1; k < N; k++)
			{
				void	*temp = numeric_mul(A->data[j * A->cols + i],
											A->data[i * A->cols + k]);
				numeric_isub(&A->data[j * A->cols + k], temp);
				delete(temp);
			}
		}
	}
	delete(absA);
	delete(maxA);
	delete(Tol);
	return (true);
}

/* INPUT: A,P filled in LUP_decompose; b - rhs vector; N - dimension
 * OUTPUT: x - solution vector of A*x=b
 *
 * Reference: https://en.wikipedia.org/wiki/LU_decomposition
 */
static struct s_Vector	*LUP_solve(
	struct s_Matrix *A, size_t *P, const struct s_Vector *b)
{
	struct s_Vector	*x = copy(b);
	size_t			N = A->cols;
	size_t			i;
	size_t			k;

	for (i = 0; i < N; i++)
	{
		delete(x->data[i]);
		x->data[i] = copy(Vector_at(b, P[i]));

		for (k = 0; k < i; k++)
		{
			void	*temp = numeric_mul(Matrix_at(A, i, k), Vector_at(x, k));

			numeric_isub(&x->data[i], temp);
			delete(temp);
		}
	}

	for (i = N - 1; i + 1 > 0; i--)
	{
		for (k = i + 1; k < N; k++)
		{
			void	*temp = numeric_mul(Matrix_at(A, i, k), Vector_at(x, k));

			numeric_isub(&x->data[i], temp);
			delete(temp);
		}
		numeric_idiv(&x->data[i], Matrix_at(A, i, i));
	}

	return (x);
}

/* Solve the system of equations A * x = b */
void	*Matrix_solve(const void *_self, const void *b)
{
	const struct s_Matrix	*self = _self;
	struct s_Matrix			*A;
	struct s_Vector			*retval = NULL;
	size_t					*P;

	if (self->rows != self->cols)
	{
		fprintf(stderr, "%s\n", "Matrix_solve: input is not square.");
		return (NULL);
	}
	if (self->cols != Vector_size(b))
	{
		fprintf(stderr, "%s\n",
				"Matrix_solve: incompatible Matrix and Vector size.");
		return (NULL);
	}
	A = copy(self);
	P = calloc(sizeof(size_t), self->cols + 1);
	if (!P)
		return (NULL);
	if (LUP_decompose(A, P))
		retval = LUP_solve(A, P, b);
	delete(A);
	free(P);
	return (retval);
}

/* INPUT: A,P filled in LUP_decompose; N - dimension
 * OUTPUT: IA is the inverse of the initial matrix
 *
 * Reference: https://en.wikipedia.org/wiki/LU_decomposition
 */
static struct s_Matrix	*LUP_invert(struct s_Matrix *A, size_t *P)
{
	size_t			N = A->cols;
	struct s_Matrix	*IA = Matrix_init(A, N, N);
	size_t			i;
	size_t			j;
	size_t			k;

	if (!IA)
		return (NULL);

	for (j = 0; j < N; j++)
	{
		for (i = 0; i < N; i++)
		{
			IA->data[i * N + j] = new(Rational, RATIONAL, P[i] == j ? 1 : 0, 1);

			for (k = 0; k < i; k++)
			{
				void	*temp;

				temp = numeric_mul(Matrix_at(A, i, k), Matrix_at(IA, k, j));
				numeric_isub(&IA->data[i * N + j], temp);
				delete(temp);
			}
		}

		for (i = N - 1; i + 1 > 0; i--)
		{
			for (k = i + 1; k < N; k++)
			{
				void	*temp;

				temp = numeric_mul(Matrix_at(A, i, k), Matrix_at(IA, k, j));
				numeric_isub(&IA->data[i * N + j], temp);
				delete(temp);
			}

			numeric_idiv(&IA->data[i * N + j], Matrix_at(A, i, i));
		}
	}

	return (IA);
}

/* Return the inverse of a Matrix. */
void	*Matrix_invert(const void *_self)
{
	const struct s_Matrix	*self = _self;
	struct s_Matrix			*A;
	struct s_Matrix			*retval = NULL;
	size_t					*P;

	if (self->rows != self->cols)
	{
		fprintf(stderr, "%s\n", "Matrix_invert: input is not square.");
		return (NULL);
	}
	A = copy(self);
	P = calloc(sizeof(size_t), self->cols + 1);
	if (!P)
		return (NULL);
	if (LUP_decompose(A, P))
		retval = LUP_invert(A, P);
	delete(A);
	free(P);
	return (retval);
}

/* INPUT: A,P filled in LUPDecompose; N - dimension.
 * OUTPUT: Function returns the determinant of the initial matrix
 *
 * Reference: https://en.wikipedia.org/wiki/LU_decomposition
 */
void	*LUP_determinant(struct s_Matrix *A, size_t *P)
{
	size_t	N = A->cols;
	void	*det = copy(Matrix_at(A, 0, 0));
	size_t	i;

	for (i = 1; i < N; i++)
		numeric_imul(&det, Matrix_at(A, i, i));

	return (P[N] - N) % 2 == 0 ? det : numeric_ineg(&det);
}

void	*Matrix_determinant(const struct s_Matrix *_self)
{
	const struct s_Matrix	*self = _self;
	struct s_Matrix			*A;
	void					*retval = NULL;
	size_t					*P;

	if (self->rows != self->cols)
	{
		fprintf(stderr, "%s\n", "Matrix_determinant: input is not square.");
		return (NULL);
	}
	A = copy(self);
	P = calloc(sizeof(size_t), self->cols + 1);
	if (!P)
		return (NULL);
	if (LUP_decompose(A, P))
		retval = LUP_determinant(A, P);
	delete(A);
	free(P);
	return (retval);
}