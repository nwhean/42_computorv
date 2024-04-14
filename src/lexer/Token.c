#include <assert.h>

#include "Token.h"
#include "Token.r"

const void	*Token;
const void	*TokenClass;

/* Token constructor method. */
static void	*Token_ctor(void *_self, va_list *app)
{
	struct s_Token	*self;

	self = super_ctor(Token, _self, app);
	self->tag = va_arg(*app, int);
	return (self);
}

/* Return a copy of the Token or its subclass. */
void	*token_copy(const void *self)
{
	const struct s_TokenClass *const	*cp = self;

	assert(self && *cp && (*cp)->copy);
	return ((*cp)->copy(self));
}

void	*super_token_copy(const void *_class, const void *_self)
{
	const struct s_TokenClass	*superclass = super(_class);

	assert(_self && superclass->copy);
	return (superclass->copy(_self));
}

/* Return a copy of the Token. */
static struct s_Token	*Token_copy(const void *_self)
{
	const struct s_Token	*self = _self;

	return new(Token, self->tag);
}

/* Return string representing the Token and its subclasses. */
const char	*token_to_string(const void *self)
{
	const struct s_TokenClass *const	*cp = self;

	assert(self && *cp && (*cp)->to_string);
	return ((*cp)->to_string(self));
}

const char	*super_token_to_string(const void *_class, const void *_self)
{
	const struct s_TokenClass	*superclass = super(_class);

	assert(_self && superclass->to_string);
	return (superclass->to_string(_self));
}

/* Return string representing the Token. */
static const char	*Token_to_string(const void *_self)
{
	const struct s_Token	*self = _self;
	char 					*retval;

	retval = calloc(sizeof(char), 2);
	if (retval)
		*retval = (char)self->tag;
	return retval;
}

/* Return the tag of the Token. */
enum e_Tag	token_get_tag(const void *_self)
{
	const struct s_Token	*self = _self;

	return (self->tag);
}

/* TokenClass constructor method. */
static void	*TokenClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_TokenClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(TokenClass, _self, app);
	va_copy(ap, *app);
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		method = va_arg(ap, voidf);
		if (selector == (voidf)token_copy)
			*(voidf *)&self->copy = method;
		else if (selector == (voidf)token_to_string)
			*(voidf *)&self->to_string = method;
	}
	return (self);
}

void	initToken(void)
{
	if (!TokenClass)
		TokenClass = new(Class, "TokenClass",
				Class, sizeof(struct s_TokenClass),
				ctor, TokenClass_ctor,
				0);
	if (!Token)
		Token = new(TokenClass, "Token",
				Object, sizeof(struct s_Token),
				ctor, Token_ctor,
				token_copy, Token_copy,
				token_to_string, Token_to_string,
				0);
}
