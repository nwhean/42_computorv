#include <stdlib.h>
#include <string.h>

#include "Num.h"

const void	*Num;

/* Num constructor method. */
static void	*Num_ctor(void *_self, va_list *app)
{
	struct s_Num	*self;

	self = super_ctor(Num, _self, app);
	self->value = va_arg(*app, int);
	return (self);
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

void	initNum(void)
{
	if (!Num)
	{
		initToken();
		Num = new(TokenClass, "Num",
				Token, sizeof(struct s_Num),
				ctor, Num_ctor,
				token_to_string, Num_to_string,
				0);
	}
}
