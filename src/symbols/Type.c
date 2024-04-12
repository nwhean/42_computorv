#include "Token.r"
#include "Word.h"
#include "Type.h"

const void	*Type;
const void	*Type_Int;
const void	*Type_Real;

/* Type constructor method. */
static void	*Type_ctor(void *_self, va_list *app)
{
	struct s_Type	*self;

	self = super_ctor(Type, _self, app);
	self->width = va_arg(*app, int);
	return (self);
}

/* Type destructor method. */
static void	*Type_dtor(void *_self)
{
	struct s_Word	*self;

	self = super_dtor(Type, _self);
	return (self);
}

/* Return string representing the Word. */
static const char	*Type_to_string(const void *_self)
{
	const struct s_Word			*self = _self;
	const struct s_TokenClass	*class = classOf(self);

	return super_token_to_string(class, self);
}

/* Return true if p is a numeric type, false otherwise. */
bool			type_numeric(struct s_Type *p)
{
	if (p == Type_Int || p == Type_Real)
		return true;
	return false;
}

/* Return the larger type of p1 and p2. */
const struct s_Type	*type_max(struct s_Type *p1, struct s_Type *p2)
{
	if (!type_numeric(p1) || !type_numeric(p2))
		return NULL;
	if (p1 == Type_Real || p2 == Type_Real)
		return Type_Real;
	return Type_Int;
}

void	initType(void)
{
	if (!Type)
	{
		initToken();
		Type = new(TokenClass, "Type",
				Word, sizeof(struct s_Type),
				ctor, Type_ctor,
				dtor, Type_dtor,
				token_to_string, Type_to_string,
				0);
		Type_Int = new(Type, BASIC, "int", sizeof(int));
		Type_Real = new(Type, BASIC, "double", sizeof(double));
	}
}
