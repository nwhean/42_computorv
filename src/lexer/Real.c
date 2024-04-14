#include <stdio.h>

#include "Num.h"
#include "Real.h"

const void	*Real;

/* Real constructor method. */
static void	*Real_ctor(void *_self, va_list *app)
{
	struct s_Real	*self;

	self = super_ctor(Real, _self, app);
	self->value = va_arg(*app, double);
	return (self);
}

/* Return a copy of the Real. */
static struct s_Real	*Real_copy(const void *_self)
{
	const struct s_Real	*self = _self;

	return new(Real, get_tag(self), self->value);
}

/* Return string representing the Real. */
static const char	*Real_to_string(const void *_self)
{
	const struct s_Real	*self = _self;
	const int			len = snprintf(NULL, 0, "%f", self->value);
	char				*retval = malloc(len + 1);

	snprintf(retval, len + 1, "%f", self->value);
	return (retval);
}

/* Return the addition of two Numerics. */
static struct s_Numeric	*Real_add(const void *_self, const void *_other)
{
	const struct s_Real		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return numeric_add(other, self);
		case REAL:
			return (new(Real, REAL,
						self->value + ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return the subtraction of one Numeric from another. */
static struct s_Numeric	*Real_sub(const void *_self, const void *_other)
{
	const struct s_Real		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return (new(Real, REAL,
						self->value - ((struct s_Num *)other)->value));
		case REAL:
			return (new(Real, REAL,
						self->value - ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return the multiplication of two Numerics. */
static struct s_Numeric	*Real_mul(const void *_self, const void *_other)
{
	const struct s_Real		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return numeric_mul(other, self);
		case REAL:
			return (new(Real, REAL,
						self->value * ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return the division of one Numeric from another. */
static struct s_Numeric	*Real_div(const void *_self, const void *_other)
{
	const struct s_Real		*self = _self;
	const struct s_Token	*other = _other;

	switch (other->tag)
	{
		case NUM:
			return (new(Real, REAL,
						self->value / ((struct s_Num *)other)->value));
		case REAL:
			return (new(Real, REAL,
						self->value / ((struct s_Real *)other)->value));
		default:
			return (NULL);
	};
}

/* Return a copy of the Real with its value negated. */
static struct s_Real	*Real_unary(const void *_self)
{
	const struct s_Real	*self = _self;
	struct s_Real		*retval = token_copy(self);

	retval->value *= -1;
	return (retval);
}

void	initReal(void)
{
	if (!Real)
	{
		initNumeric();
		Real = new(NumericClass, "Real",
				Numeric, sizeof(struct s_Real),
				ctor, Real_ctor,
				token_copy, Real_copy,
				token_to_string, Real_to_string,
				numeric_add, Real_add,
				numeric_sub, Real_sub,
				numeric_mul, Real_mul,
				numeric_div, Real_div,
				numeric_unary, Real_unary,
				0);
	}
}
