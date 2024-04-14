#include <stdlib.h>
#include <string.h>

#include "Num.h"
#include "Real.h"

const void	*Num;

/* Num constructor method. */
static void	*Num_ctor(void *_self, va_list *app)
{
	struct s_Num	*self;

	self = super_ctor(Num, _self, app);
	self->value = va_arg(*app, int);
	return (self);
}

/* Return a copy of the Num. */
static struct s_Num	*Num_copy(const void *_self)
{
	const struct s_Num	*self = _self;

	return new(Num, get_tag(self), self->value);
}

/* Return string representing the Num. */
static const char	*Num_to_string(const void *_self)
{
	const struct s_Num	*self = _self;
	const int			len = snprintf(NULL, 0, "%d", self->value);
	char				*retval = malloc(len + 1);

	snprintf(retval, len + 1, "%d", self->value);
	return (retval);
}

/* Return the addition of two Numerics. */
static struct s_Numeric	*Num_add(const void *_self, const void *_other)
{
	const struct s_Num		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return (new(Num, NUM,
						self->value + ((struct s_Num *)other)->value));
		case REAL:
			return (new(Real, REAL,
						self->value + ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return the subtraction of one Numeric from another. */
static struct s_Numeric	*Num_sub(const void *_self, const void *_other)
{
	const struct s_Num		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return (new(Num, NUM,
						self->value - ((struct s_Num *)other)->value));
		case REAL:
			return (new(Real, REAL,
						self->value - ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return the multiplication of two Numerics.. */
static struct s_Numeric	*Num_mul(const void *_self, const void *_other)
{
	const struct s_Num		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return (new(Num, NUM,
						self->value * ((struct s_Num *)other)->value));
		case REAL:
			return (new(Real, REAL,
						self->value * ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return the division of one Numeric from another. */
static struct s_Numeric	*Num_div(const void *_self, const void *_other)
{
	const struct s_Num		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return (new(Real, REAL,
						(double)self->value / ((struct s_Num *)other)->value));
		case REAL:
			return (new(Real, REAL,
						self->value / ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return a copy of the Num with its value negated. */
static struct s_Num	*Num_unary(const void *_self)
{
	const struct s_Num	*self = _self;
	struct s_Num		*retval = (struct s_Num *)token_copy(self);

	retval->value *= -1;
	return (retval);
}

void	initNum(void)
{
	if (!Num)
	{
		initNumeric();
		Num = new(NumericClass, "Num",
				Numeric, sizeof(struct s_Num),
				ctor, Num_ctor,
				token_copy, Num_copy,
				token_to_string, Num_to_string,
				numeric_add, Num_add,
				numeric_sub, Num_sub,
				numeric_mul, Num_mul,
				numeric_div, Num_div,
				numeric_unary, Num_unary,
				0);
	}
}
