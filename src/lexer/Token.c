#include <assert.h>

#include "Token.h"

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

/* Return a copy of the Token. */
static struct s_Token	*Token_copy(const void *_self)
{
	const struct s_Token	*self = _self;
	struct s_Token			*retval;

	retval = super_copy(Token, self);
	retval->tag = self->tag;
	return (retval);
}

/* Return string representing the Token. */
static char	*Token_str(const void *_self)
{
	const struct s_Token	*self = _self;
	char 					*retval;

	retval = calloc(sizeof(char), 2);
	if (retval)
		*retval = (char)self->tag;
	return retval;
}

/* Return the equality of Token. */
static bool	Token_equal(const void *_self, const void *_other)
{
	const struct s_Token	*self = _self;
	const struct s_Token	*other = _other;

	if (!super_equal(Token, _self, _other))
		return (false);
	return (self->tag == other->tag);
}

/* TokenClass constructor method. */
static void	*TokenClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_TokenClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(TokenClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		(void)method;
		#pragma GCC diagnostic pop
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
				copy, Token_copy,
				str, Token_str,
				equal, Token_equal,
				0);
}

/* return the tag of a Token. */
enum e_Tag	Token_get_tag(const void *_self)
{
	const struct s_Token	*self = _self;

	return (self->tag);
}
