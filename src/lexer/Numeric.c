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

/* Return a copy of the Numeric with its value negated. */
void	*numeric_add(const void *self, const void *other)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->add);
	return ((*cp)->add(self, other));
}

/* Return a copy of the Numeric with its value negated. */
void	*numeric_unary(const void *self)
{
	const struct s_NumericClass *const	*cp = self;

	assert(self && *cp && (*cp)->unary);
	return ((*cp)->unary(self));
}

/* NumericClass constructor method. */
static void	*NumericClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_NumericClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(NumericClass, _self, app);
	va_copy(ap, *app);
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		method = va_arg(ap, voidf);
		if (selector == (voidf)numeric_add)
			*(voidf *)&self->add = method;
		else if (selector == (voidf)numeric_unary)
			*(voidf *)&self->unary = method;
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
				0);
}
