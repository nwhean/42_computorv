#include <assert.h>

#include "Numeric.h"

const void	*Numeric;
const void	*NumericClass;

/* Numeric constructor method. */
static void	*Numeric_ctor(void *_self, va_list *app)
{
	struct s_Numeric	*self;

	self = super_ctor(Numeric, _self, app);
	return (self);
}

/* Return a copy of the Numeric. */
static struct s_Numeric	*Numeric_copy(const void *_self)
{
	return (super_copy(Numeric, _self));
}

/* Return the addition of two Numerics. */
void	*numeric_add(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->add);
	return ((*cp)->add(self, other));
}

/* Return the subtraction of one Numeric from aother. */
void	*numeric_sub(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->sub);
	return ((*cp)->sub(self, other));
}

/* Return the multiplication of two Numerics. */
void	*numeric_mul(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->mul);
	return ((*cp)->mul(self, other));
}

/* Return the division of one Numeric from another. */
void	*numeric_div(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->div);
	return ((*cp)->div(self, other));
}

/* Return the remainder from the division of one Numeric from another. */
void	*numeric_mod(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->mod);
	return ((*cp)->mod(self, other));
}

/* Return a copy of the Numeric with its value negated. */
void	*numeric_pos(const void *self)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->pos);
	return ((*cp)->pos(self));
}

/* Return a copy of the Numeric with its value negated. */
void	*numeric_neg(const void *self)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->neg);
	return ((*cp)->neg(self));
}

/* Return the exponentiation of one Numeric to another. */
void	*numeric_pow(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->pow);
	return ((*cp)->pow(self, other));
}

/* Return true if two numerics are the same, false otherwise. */
bool	numeric_equal(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->equal);
	return ((*cp)->equal(self, other));
}

/* Promote one Numeric type to another */
void	*numeric_promote(const void *self, enum e_Tag tag)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->promote);
	return ((*cp)->promote(self, tag));
}

/* NumericClass constructor method. */
static void	*NumericClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_NumericClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(NumericClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		method = va_arg(ap, voidf);
		if (selector == (voidf)numeric_add)
			*(voidf *)&self->add = method;
		else if (selector == (voidf)numeric_sub)
			*(voidf *)&self->sub = method;
		else if (selector == (voidf)numeric_mul)
			*(voidf *)&self->mul = method;
		else if (selector == (voidf)numeric_div)
			*(voidf *)&self->div = method;
		else if (selector == (voidf)numeric_mod)
			*(voidf *)&self->mod = method;
		else if (selector == (voidf)numeric_pos)
			*(voidf *)&self->pos = method;
		else if (selector == (voidf)numeric_neg)
			*(voidf *)&self->neg = method;
		else if (selector == (voidf)numeric_pow)
			*(voidf *)&self->pow = method;
		else if (selector == (voidf)numeric_equal)
			*(voidf *)&self->equal = method;
		else if (selector == (voidf)numeric_promote)
			*(voidf *)&self->promote = method;
		#pragma GCC diagnostic pop
	}
	return (self);
}

void	initNumeric(void)
{
	initToken();
	if (!NumericClass)
		NumericClass = new(Class, "NumericClass",
				TokenClass, sizeof(struct s_NumericClass),
				ctor, NumericClass_ctor,
				0);
	if (!Numeric)
		Numeric = new(NumericClass, "Numeric",
				Token, sizeof(struct s_Numeric),
				ctor, Numeric_ctor,
				copy, Numeric_copy,
				0);
}

bool	numeric_is(enum e_Tag tag)
{
	if (tag == RATIONAL || tag == COMPLEX)
		return true;
	return false;
}

enum e_Tag	numeric_max(enum e_Tag tag1, enum e_Tag tag2)
{
	if (!numeric_is(tag1) || !numeric_is(tag2))
		return (0);
	if (tag1 == COMPLEX || tag2 == COMPLEX)
		return (COMPLEX);
	return (RATIONAL);
}
