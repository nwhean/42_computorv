#include "Token.h"

/* Token constructor method. */
static void	*Token_ctor(void *_self, va_list *app)
{
	struct s_Token	*self;

	self = super_ctor(Token, _self, app);
	self->tag = va_arg(*app, int);
	return (self);
}

const void	*Token;

void	initToken(void)
{
	if (!Token)
		Token = new(Class, "Token",
				Object, sizeof(struct s_Token),
				ctor, Token_ctor,
				0);
}
