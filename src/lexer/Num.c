#include "Num.h"

/* Num constructor method. */
static void	*Num_ctor(void *_self, va_list *app)
{
	struct s_Num	*self;

	self = super_ctor(Num, _self, app);
	self->value = va_arg(*app, int);
	return (self);
}

const void	*Num;

void	initNum(void)
{
	if (!Num)
	{
		initToken();
		Num = new(Class, "Num",
				Token, sizeof(struct s_Num),
				ctor, Num_ctor,
				0);
	}
}
