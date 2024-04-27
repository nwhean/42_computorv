#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Vec.h"

/* lexer */
#include "Complex.h"
#include "Rational.h"
#include "Vector.h"

const void	*Vector;

/* Vector constructor method. */
static void	*Vector_ctor(void *_self, va_list *app)
{
	struct s_Vector	*self;

	self = super_ctor(Vector, _self, app);
	self->vec = va_arg(*app, void *);
	return (self);
}

/* Return a copy of the Vector. */
static struct s_Vector	*Vector_copy(const void *_self)
{
	const struct s_Vector	*self = _self;
	struct s_Vector			*retval;

	retval = super_copy(Vector, self);
	retval->vec = copy(self->vec);
	return (retval);
}

/* Vector destructor method. */
static void	*Vector_dtor(void *_self)
{
	struct s_Vector	*self = _self;

	delete(self->vec);
	return(super_dtor(Vector, _self));
}

/* Return string representing the Vector. */
static char	*Vector_str(const void *_self)
{
	const struct s_Vector	*self = _self;

	return (str(self->vec));
}

/* Element-by-element operation between a Vector and a Scalar.*/
static void	*Vector_op_Scalar(const void *_self,
							const void *_other,
							void* (*func)(const void *, const void *))
{
	const struct s_Vector	*self = _self;
	size_t					size = Vector_size(self);
	struct s_Vector			*retval = new(Vector, VECTOR, new(Vec));
	size_t					i;

	for (i = 0; i < size; ++i)
		Vec_push_back(retval->vec, func(Vector_at(self, i), _other));
	return (retval);
}

/* Element-by-element operation between two Vectors.*/
static void	*Vector_op_Vector(const void *_self,
							const void *_other,
							void* (*func)(const void *, const void *))
{
	const struct s_Vector	*self = _self;
	const struct s_Vector	*other = _other;
	size_t					size = Vector_size(self);
	size_t					i;
	struct s_Vector			*retval;

	if (size != Vector_size(other))
	{
		fprintf(stderr, "%s\n", "Vector_op_Vector: unequal sizes.");
		return (NULL);
	}
	retval = new(Vector, VECTOR, new(Vec));
	for (i = 0; i < size; ++i)
	{
		Vec_push_back(retval->vec, func(Vector_at(self, i),
										Vector_at(other, i)));
	}
	return (retval);
}

/* Return the addition of two Numerics. */
static void	*Vector_add(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
			fprintf(stderr, "%s\n", "Vector_add: incompatible with Rational.");
			return (NULL);
		case COMPLEX:
			fprintf(stderr, "%s\n", "Vector_add: incompatible with Complex.");
			return (NULL);
		case VECTOR:
			return Vector_op_Vector(_self, _other, numeric_add);
	};
	return (NULL);
}

/* Return the subtraction of one Numeric from another. */
static void	*Vector_sub(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
			fprintf(stderr, "%s\n", "Vector_sub: incompatible with Rational.");
			return (NULL);
		case COMPLEX:
			fprintf(stderr, "%s\n", "Vector_sub: incompatible with Complex.");
			return (NULL);
		case VECTOR:
			return Vector_op_Vector(_self, _other, numeric_sub);
	};
	return (NULL);
}

/* Return the multiplication of two Numerics. */
static void	*Vector_mul(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			return Vector_op_Scalar(_self, _other, numeric_mul);
		case VECTOR:
			return Vector_op_Vector(_self, _other, numeric_mul);
	};
	return (NULL);
}

/* Return the division of one Numeric from another. */
static void	*Vector_div(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			return Vector_op_Scalar(_self, _other, numeric_div);
		case VECTOR:
			return Vector_op_Vector(_self, _other, numeric_div);
	};
	return (NULL);
}

/* Return remainder from the division of one Numeric from another. */
static void	*Vector_mod(const void *_self, const void *_other)
{
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case RATIONAL:
		case COMPLEX:
			return Vector_op_Scalar(_self, _other, numeric_mod);
		case VECTOR:
			fprintf(stderr, "%s\n", "Vector_mod: incompatible with Vector.");
			return (NULL);
	};
	return (NULL);
}

/* Return a copy of the Vector with its value negated. */
static struct s_Vector	*Vector_neg(const void *_self)
{
	const struct s_Vector	*self = _self;
	struct s_Vector			*retval = new(Vector, VECTOR, new(Vec));
	size_t					size = Vector_size(self);
	size_t					i;

	for (i = 0; i < size; ++i)
		Vec_push_back(retval->vec, numeric_neg(Vector_at(self, i)));
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

	if (!super_equal(Vector, _self, _other))
		return (false);
	if (!equal(self->vec, other->vec))
		return (false);
	return (true);
}

/* Promote one Numeric type to another */
static void	*Vector_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Vector	*self = _self;

	switch (tag)
	{
		case RATIONAL:
		case COMPLEX:
			fprintf(stderr, "%s\n", "Cannot demote Vector to scalar type.");
			return (NULL);
		case VECTOR:
			return Vector_copy(self);
		default:
			fprintf(stderr, "%s\n", "Other Vector promotion is not supported");
			return (NULL);
	};
}

void	initVector(void)
{
	initVec();
	initComplex();
	initNumeric();
	initRational();
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
	}
}

/* Return the size of the Vector. */
size_t	Vector_size(const void *_self)
{
	const struct s_Vector	*self = _self;

	return (Vec_size(self->vec));
}

/* Returns a reference to the element at position `n` in the vector. */
void	*Vector_at(const void *_self, size_t n)
{
	const struct s_Vector	*self = _self;

	return (Vec_at(self->vec, n));
}

/* Return the vector containing the conjugate of each of its elements. */
struct s_Vector	*Vector_conjugate(const void *_self)
{
	const struct s_Vector	*self = _self;
	struct s_Vector			*retval = new(Vector, VECTOR, new(Vec));
	size_t					size = Vector_size(self);
	size_t					i;

	for (i = 0; i < size; ++i)
	{
		struct s_Token	*elem = Vector_at(self, i);
		if (elem->tag == RATIONAL)
			Vec_push_back(retval->vec, copy((void *)elem));
		else if (elem->tag == COMPLEX)
			Vec_push_back(retval->vec, Complex_conjugate((void *)elem));
		else
		{
			fprintf(stderr, "%s\n", "Vector_conjugate: Unexpected input type");
			delete(retval);
			return (NULL);
		}
	}
	return (retval);
}

/* Return the cross product of a two vectors. */
struct s_Vector	*Vector_cross(const void *_self, const void *_other)
{
	const struct s_Vector	*self = _self;
	const struct s_Vector	*other = _other;
	size_t					size = Vector_size(self);
	struct s_Vector			*retval;
	struct s_Vector			*conj;
	size_t					i;
	size_t					j;
	size_t					k;
	void					*temp0;
	void					*temp1;
	void					*temp2;

	if (size != 3 || size != Vector_size(other))
	{
		fprintf(stderr, "%s\n", "Vector_cross: vectors must be of len 3.");
		return (NULL);
	}
	conj = new(Vector, VECTOR, new(Vec));
	for (i = 0; i < size; ++i)
	{
		j = (i + 1) % 3;
		k = (i + 2) % 3;
		temp0 = numeric_mul(Vector_at(self, j), Vector_at(other, k));
		temp1 = numeric_mul(Vector_at(self, k), Vector_at(other, j));
		temp2 = numeric_sub(temp0, temp1);
		Vec_push_back(conj->vec, copy(temp2));
		delete(temp0);
		delete(temp1);
		delete(temp2);
	}
	retval = Vector_conjugate(conj);
	delete(conj);
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
	void					*temp;

	sum = new(Rational, RATIONAL, 0, 1);
	for (i = 0; i < size; ++i)
	{
		temp = numeric_add(sum, Vector_at(self, i));
		delete(sum);
		sum = temp;
	}
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
