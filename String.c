#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "new.r"
#include "String.h"

static void	*String_ctor(void *_self, va_list *app)
{
	struct s_String	*self;
	const char		*text = va_arg(*app, const char *);

	self = _self;
	self->text = malloc(strlen(text) + 1);
	assert(self->text);
	strcpy(self->text, text);
	return (self);
}

static void	*String_dtor(void *_self)
{
	struct s_String	*self;

	self = _self;
	free(self->text);
	self->text = 0;
	return (self);
}

static void	*String_clone(const void *_self)
{
	const struct s_String	*self = _self;

	return (new(String, self->text));
}

static int	String_differ(const void *_self, const void *_b)
{
	const struct s_String	*self = _self;
	const struct s_String	*b = _b;

	if (self == b)
		return (0);
	if (! b || b->class != String)
		return (1);
	return (strcmp(self->text, b->text));
}

static const struct s_Class	_String = {
	sizeof(struct s_String),
	String_ctor,
	String_dtor,
	String_clone,
	String_differ
};

const void					*String = &_String;
