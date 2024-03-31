#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "new.r"
#include "String.h"

static struct s_String		*ring;

static void	*String_ctor(void *_self, va_list *app)
{
	struct s_String	*self;
	const char		*text = va_arg(*app, const char *);
	struct s_String	*p;

	self = _self;
	if (ring)
	{
		p = ring;
		do
		{
			if (strcmp(p->text, text) == 0)
			{
				++p->count;
				free(self);
				return (p);
			}
		}
		while ((p = p->next) != ring);
	}
	else
		ring = self;
	self->next = ring->next;
	ring->next = self;
	self->count = 1;
	self->text = malloc(strlen(text) + 1);
	assert(self->text);
	strcpy(self->text, text);
	return (self);
}

static void	*String_dtor(void *_self)
{
	struct s_String	*self;
	struct s_String	*p;

	self = _self;
	if (--self->count > 0)
		return (NULL);
	assert(ring);
	if (ring == self)
		ring = self->next;
	if (ring == self)
		ring = NULL;
	else
	{
		p = ring;
		while (p->next != self)
		{
			p = p->next;
			assert(p != ring);
		}
		p->next = self->next;
	}
	free(self->text);
	self->text = 0;
	return (self);
}

static void	*String_clone(const void *_self)
{
	struct s_String	*self = (void *)_self;

	++self->count;
	return (self);
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
