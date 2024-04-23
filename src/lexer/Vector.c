#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* container */
#include "Str.h"

/* lexer */
#include "Vector.h"

const void	*Vector;

/* Vector constructor method. */
static void	*Vector_ctor(void *_self, va_list *app)
{
	size_t			i = -1;
	struct s_Vector	*self;
	void			**nums;

	self = super_ctor(Vector, _self, app);
	self->size = va_arg(*app, size_t);
	nums = calloc(sizeof(void **), self->size);
	if (!nums)
	{
		fprintf(stderr, "%s\n", "Vector_ctor: memory allocation failed.");
		return (NULL);
	}
	while (++i < self->size)
		nums[i] = va_arg(*app, void *);;
	self->nums = nums;
	return (self);
}

/* Return a copy of the Vector. */
static struct s_Vector	*Vector_copy(const void *_self)
{
	const struct s_Vector	*self = _self;
	size_t					i = -1;
	struct s_Vector			*retval;
	void					**nums;

	retval = super_copy(Vector, self);
	retval->size = self->size;
	nums = calloc(sizeof(void **), self->size);
	if (!nums)
	{
		fprintf(stderr, "%s\n", "Vector_copy: memory allocation failed.");
		return (NULL);
	}
	while (++i < self->size)
		nums[i] = copy(self->nums[i]);
	retval->nums = nums;
	return (retval);
}

/* Vector destructor method. */
static void	*Vector_dtor(void *_self)
{
	struct s_Vector	*self = _self;

	while (self->size--)
		delete(self->nums[self->size]);
	free(self->nums);
	return(super_dtor(Vector, _self));
}

/* Return string representing the Vector. */
static char	*Vector_str(const void *_self)
{
	const struct s_Vector	*self = _self;
	size_t					size = 0;
	struct s_Str			*s;
	char					*s_append;
	char					*retval;

	s = new(Str, "[");
	while (size < self->size)
	{
		s_append = str(self->nums[size]);
		Str_append(s, s_append);
		free(s_append);
		++size;
		if (size < self->size)
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
							void* (*func)(const void *, const void *))
{
	const struct s_Vector	*self = _self;
	struct s_Vector			*retval = copy(self);	/* inefficient */
	void					*num;
	size_t					i;

	for (i = 0; i < self->size; ++i)
	{
		num = func(self->nums[i], _other);
		delete(retval->nums[i]);
		retval->nums[i] = num;
	}
	return (retval);
}

/* Element-by-element operation between two Vectors.*/
static void	*Vector_op_Vector(const void *_self,
							const void *_other,
							void* (*func)(const void *, const void *))
{
	const struct s_Vector	*self = _self;
	const struct s_Vector	*other = _other;
	struct s_Vector			*retval = copy(self);	/* inefficient */
	void					*num;
	size_t					i;

	if (self->size != other->size)
	{
		fprintf(stderr, "%s\n", "Vector_op_Vector: unequal sizes.");
		return (NULL);
	}
	for (i = 0; i < self->size; ++i)
	{
		num = func(self->nums[i], other->nums[i]);
		delete(retval->nums[i]);
		retval->nums[i] = num;
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
	struct s_Vector			*retval = copy(self);
	void					*num;
	size_t					i;

	for (i = 0; i < self->size; ++i)
	{
		num = numeric_neg(self->nums[i]);
		delete(retval->nums[i]);
		retval->nums[i] = num;
	}
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
static bool	Vector_equal(
		const struct s_Vector *_self, const struct s_Vector *_other)
{
	const struct s_Token	*self = (const struct s_Token *)_self;
	const struct s_Token	*other = (const struct s_Token *)_other;
	size_t					i;

	if (self->tag != other->tag)
		return (false);
	if (_self->size != _other->size)
		return (false);
	for (i = 0; i < _self->size; ++i)
	{
		if (!numeric_equal(_self->nums[i], _other->nums[i]))
			return (false);
	}
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
	initStr();
	initNumeric();
	if (!Vector)
	{
		Vector = new(NumericClass, "Vector",
				Numeric, sizeof(struct s_Vector),
				ctor, Vector_ctor,
				copy, Vector_copy,
				dtor, Vector_dtor,
				str, Vector_str,
				numeric_add, Vector_add,
				numeric_sub, Vector_sub,
				numeric_mul, Vector_mul,
				numeric_div, Vector_div,
				numeric_mod, Vector_mod,
				numeric_pos, Vector_copy,
				numeric_neg, Vector_neg,
				numeric_pow, Vector_pow,
				numeric_equal, Vector_equal,
				numeric_promote, Vector_promote,
				0);
	}
}
