#include <stdio.h>

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

/* Return string representing the Num. */
static const char	*Real_to_string(const void *_self)
{
	const struct s_Real	*self = _self;
	const int			len = snprintf(NULL, 0, "%f", self->value);
	char				*retval = malloc(len + 1);

	snprintf(retval, len + 1, "%f", self->value);
	return (retval);
}

void	initReal(void)
{
	if (!Real)
	{
		initToken();
		Real = new(TokenClass, "Real",
				Token, sizeof(struct s_Real),
				ctor, Real_ctor,
				token_to_string, Real_to_string,
				0);
	}
}
