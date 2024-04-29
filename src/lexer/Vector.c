#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Str.h"
#include "Vec.h"

/* lexer */
#include "Complex.h"
#include "Rational.h"
#include "Vector.h"
#include "Matrix.h"

/* other */
#include "utility.h"

const void	*Vector;

/* Vector constructor method. */
static void	*Vector_ctor(void *_self, va_list *app)
{
	struct s_Vector	*self;
	void			*vec;
	void			**data;
	size_t			i;

	self = super_ctor(Vector, _self, app);
	vec = va_arg(*app, void *);
	data = Vec_data(vec);
	self->size = Vec_size(vec);
	self->data = calloc(sizeof(void *), self->size);
	if (!self->data)
	{
		delete(vec);
		delete(self);
		return (NULL);
	}
	for (i = 0; i < self->size; ++i)
		swap_ptr(&self->data[i], &data[i]);
	delete(vec);
	return (self);
}

/* Return a copy of the Vector. */
static struct s_Vector	*Vector_copy(const void *_self)
{
	const struct s_Vector	*self = _self;
	struct s_Vector			*retval;
	size_t					i;

	retval = super_copy(Vector, self);
	retval->size = self->size;
	retval->data = calloc(sizeof(void *), retval->size);
	if (retval->data == NULL)
	{
		delete(retval);
		return (NULL);
	}
	for (i = 0; i < retval->size; ++i)
		retval->data[i] = copy(self->data[i]);
	return (retval);
}

/* Vector destructor method. */
static void	*Vector_dtor(void *_self)
{
	struct s_Vector	*self = _self;
	size_t			i;

	for (i = 0; i < self->size; ++i)
		delete(self->data[i]);
	free(self->data);
	return(super_dtor(Vector, _self));
}

/* Return string representing the Vector. */
static char	*Vector_str(const void *_self)
{
	const struct s_Vector	*self = _self;
	void					*s = new(Str, "[");	/* Str */
	char					*s_append;
	char					*retval;
	size_t					i;

	for (i = 0; i < self->size; ++i)
	{
		s_append = str(self->data[i]);
		Str_append(s, s_append);
		free(s_append);
		if (i < self->size - 1)
			Str_append(s, ", ");
	}
	Str_push_back(s, ']');
	retval = str(s);
	delete(s);
	return (retval);
}

/* Element-by-element operation between a Vector and a Scalar.*/
static void	*Vector_op_Scalar(const void *_self,
							const void *_other,
							void *(*func)(const void *, const void *))
{
	const struct s_Vector	*self = _self;
	void					*vec = new(Vec);
	size_t					i;

	for (i = 0; i < self->size; ++i)
		Vec_push_back(vec, func(self->data[i], _other));
	return (new(Vector, VECTOR, vec));
}

/* Element-by-element operation between two Vectors.*/
static void	*Vector_op_Vector(const void *_self,
							const void *_other,
							void *(*func)(const void *, const void *))
{
	const struct s_Vector	*self = _self;
	const struct s_Vector	*other = _other;
	size_t					size = Vector_size(self);
	size_t					i;
	void					*vec;

	if (size != Vector_size(other))
	{
		fprintf(stderr, "%s\n", "Vector_op_Vector: unequal sizes.");
		return (NULL);
	}
	vec = new(Vec);
	for (i = 0; i < size; ++i)
		Vec_push_back(vec, func((self->data[i]), other->data[i]));
	return (new(Vector, VECTOR, vec));
}

/* Return the addition of two Numerics. */
static void	*Vector_add(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			fprintf(stderr, "%s\n", "Vector_add: incompatible with Rational.");
			return (NULL);
		case COMPLEX:
			fprintf(stderr, "%s\n", "Vector_add: incompatible with Complex.");
			return (NULL);
		case VECTOR:
			return Vector_op_Vector(_self, _other, numeric_add);
		case MATRIX:
			fprintf(stderr, "%s\n", "Vector_add: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Vector_add: unexpected input type.");
			return (NULL);
	};
}

/* Return the subtraction of one Numeric from another. */
static void	*Vector_sub(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			fprintf(stderr, "%s\n", "Vector_sub: incompatible with Rational.");
			return (NULL);
		case COMPLEX:
			fprintf(stderr, "%s\n", "Vector_sub: incompatible with Complex.");
			return (NULL);
		case VECTOR:
			return (Vector_op_Vector(_self, _other, numeric_sub));
		case MATRIX:
			fprintf(stderr, "%s\n", "Vector_sub: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Vector_sub: unexpected input type.");
			return (NULL);
	};
}

/* Return the multiplication of two Numerics. */
static void	*Vector_mul(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return (Vector_op_Scalar(_self, _other, numeric_mul));
		case VECTOR:
			return (Vector_op_Vector(_self, _other, numeric_mul));
		case MATRIX:
			fprintf(stderr, "%s\n", "Vector_mul: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Vector_mul: unexpected input type.");
			return (NULL);
	};
}

/* Return the division of one Numeric from another. */
static void	*Vector_div(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return (Vector_op_Scalar(_self, _other, numeric_div));
		case VECTOR:
			return (Vector_op_Vector(_self, _other, numeric_div));
		case MATRIX:
			fprintf(stderr, "%s\n", "Vector_div: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Vector_div: unexpected input type.");
			return (NULL);
	};
}

/* Return remainder from the division of one Numeric from another. */
static void	*Vector_mod(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return Vector_op_Scalar(_self, _other, numeric_mod);
		case VECTOR:
			fprintf(stderr, "%s\n", "Vector_mod: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Vector_mod: incompatible with Matrix.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Vector_mod: unexpected input type.");
			return (NULL);
	};
}

/* Return a copy of the Vector with its value negated. */
static struct s_Vector	*Vector_neg(const void *_self)
{
	const struct s_Vector	*self = _self;
	struct s_Vector			*retval = copy(self);
	size_t					i;

	for (i = 0; i < self->size; ++i)
		numeric_ineg(&(retval->data[i]));
	return (retval);
}

/* Return the exponentiation of one Numeric to another. */
static void	*Vector_pow(const void *_self, const void *_other)
{
	(void)_self;
	(void)_other;
	fprintf(stderr, "%s\n", "Vector_pow: Undefined.");
	return (NULL);
}

/* Return true if two Vectors are the same, false otherwise. */
static bool	Vector_equal(const void *_self, const void *_other)
{
	const struct s_Vector	*self = _self;
	const struct s_Vector	*other = _other;
	size_t					i;

	if (!super_equal(Vector, _self, _other))
		return (false);
	if (self->size != other->size)
		return (false);
	for (i = 0; i < self->size; ++i)
	{
		if (!equal(self->data[i], other->data[i]))
			return (false);
	}
	return (true);
}

/* Promote one Numeric type to another */
static void	*Vector_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Vector	*self = _self;
	void					*vec_v;
	void					*vec_m;
	size_t					i;

	switch (tag)
	{
		case RATIONAL:
		case COMPLEX:
			fprintf(stderr, "%s\n",
					"Vector_promotion: cannot demote Vector to scalar type.");
			return (NULL);
		case VECTOR:
			return Vector_copy(self);
		case MATRIX:
			vec_v = new(Vec);
			for (i = 0; i < self->size; ++i)
				Vec_push_back(vec_v, copy(self->data[i]));
			vec_m = new(Vec);
			Vec_push_back(vec_m, vec_v);
			return (new(Matrix, MATRIX, vec_m));
		default:
			fprintf(stderr, "%s\n",
					"Vector_promotion: unexpected input type.");
			return (NULL);
	};
}

void	initVector(void)
{
	initNumeric();
	if (!Vector)
	{
		Vector = new(NumericClass, "Vector",
				Numeric, sizeof(struct s_Vector),
				ctor, Vector_ctor,
				copy, Vector_copy,
				dtor, Vector_dtor,
				str, Vector_str,
				equal, Vector_equal,
				numeric_add, Vector_add,
				numeric_sub, Vector_sub,
				numeric_mul, Vector_mul,
				numeric_div, Vector_div,
				numeric_mod, Vector_mod,
				numeric_neg, Vector_neg,
				numeric_pow, Vector_pow,
				numeric_promote, Vector_promote,
				0);
		initStr();
		initVec();
		initRational();
		initComplex();
		initMatrix();
	}
}

/* Return the size of the Vector. */
size_t	Vector_size(const void *_self)
{
	const struct s_Vector	*self = _self;

	return (self->size);
}

/* Returns a reference to the element at position `n` in the vector. */
void	*Vector_at(const void *_self, size_t n)
{
	const struct s_Vector	*self = _self;

	return (self->data[n]);
}

/* Return the vector containing the conjugate of each of its elements. */
struct s_Vector	*Vector_conjugate(const void *_self)
{
	const struct s_Vector	*self = _self;
	void					*vec = new(Vec);
	struct s_Vector			*retval;
	size_t					i;

	for (i = 0; i < self->size; ++i)
	{
		void	*elem = self->data[i];
		if (Token_get_tag(elem) == RATIONAL)
			Vec_push_back(vec, copy(elem));
		else if (Token_get_tag(elem) == COMPLEX)
			Vec_push_back(vec, Complex_conjugate(elem));
		else
		{
			fprintf(stderr, "%s\n", "Vector_conjugate: Unexpected input type");
			delete(vec);
			return (NULL);
		}
	}
	retval = new(Vector, VECTOR, vec);
	return (retval);
}

/* Return the cross product of a two vectors. */
struct s_Vector	*Vector_cross(const void *_self, const void *_other)
{
	const struct s_Vector	*self = _self;
	const struct s_Vector	*other = _other;
	void					*conj;
	struct s_Vector			*retval;
	size_t					i;
	size_t					j;
	size_t					k;
	void					*temp0;
	void					*temp1;
	void					*temp2;

	if (self->size != 3 || self->size != other->size)
	{
		fprintf(stderr, "%s\n", "Vector_cross: vectors must be of len 3.");
		return (NULL);
	}
	conj = new(Vec);
	for (i = 0; i < self->size; ++i)
	{
		j = (i + 1) % 3;
		k = (i + 2) % 3;
		temp0 = numeric_mul(Vector_at(self, j), Vector_at(other, k));
		temp1 = numeric_mul(Vector_at(self, k), Vector_at(other, j));
		temp2 = numeric_sub(temp0, temp1);
		Vec_push_back(conj, Complex_conjugate(temp2));
		delete(temp0);
		delete(temp1);
		delete(temp2);
	}
	retval = new(Vector, VECTOR, conj);
	return (retval);
}

/* Return the dot product of two possibly complex vectors. */
struct s_Rational	*Vector_dot(const void *_self, const void *_other)
{
	const struct s_Vector		*self = _self;
	const struct s_Vector		*other = _other;
	size_t						size = Vector_size(self);
	struct s_Vector				*conj;
	struct s_Vector				*v;
	struct s_Rational			*retval;

	if (size != Vector_size(other))
	{
		fprintf(stderr, "%s\n", "Vector_dot: unequal sizes.");
		return (NULL);
	}
	conj = Vector_conjugate(other);
	v = numeric_mul(self, conj);
	retval = Vector_sum(v);
	delete(conj);
	delete(v);
	return (retval);
}

/* Return the magnitude of a vector. */
struct s_Rational	*Vector_magnitude(const void *_self)
{
	const struct s_Vector	*self = _self;
	struct s_Vector			*conj = Vector_conjugate(self);
	struct s_Rational		*dot = Vector_dot(self, conj);
	double					val;

	val = Rational_to_double(dot);
	val = sqrt(val);	/* should implement my own sqrt function */
	delete(conj);
	delete(dot);
	return (Rational_from_double(val));
}

/* Return the sum of the element of the vector. */
void				*Vector_sum(const void *_self)
{
	const struct s_Vector	*self = _self;
	size_t					size = Vector_size(self);
	size_t					i;
	void					*sum;

	sum = new(Rational, RATIONAL, 0, 1);
	for (i = 0; i < size; ++i)
		numeric_iadd(&sum, Vector_at(self, i));
	return (sum);
}

/* Return the sum of the product of two vectors. */
void	*Vector_sum_product(const void *_self, const void *_other)
{
	const struct s_Vector		*self = _self;
	const struct s_Vector		*other = _other;
	size_t						size = Vector_size(self);
	struct s_Vector				*v;
	struct s_Rational			*retval;

	if (size != Vector_size(other))
	{
		fprintf(stderr, "%s\n", "Vector_sum_product: unequal sizes.");
		return (NULL);
	}
	v = numeric_mul(self, other);
	retval = Vector_sum(v);
	delete(v);
	return (retval);
}
