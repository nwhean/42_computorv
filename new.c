#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "new.r"

void	*new(const void *_class, ...)
{
	const struct s_Class	*class = _class;
	void					*p;
	va_list					ap;

	p = calloc(1, class->size);
	assert(p);
	*(const struct s_Class **)p = class;
	if (class->ctor)
	{
		va_start(ap, _class);
		p = class->ctor(p, &ap);
		va_end(ap);
	}
	return (p);
}

void	delete(void *self)
{
	const struct s_Class	**cp = self;

	if (self && *cp && (*cp)->dtor)
		self = (*cp)->dtor(self);
	free(self);
}

void	draw(const void *self)
{
	const struct s_Class *const	*cp = self;

	if (self && *cp && (*cp)->draw)
		(*cp)->draw(self);
}

size_t	sizeOf(const void *self)
{
	const struct s_Class *const	*cp = self;

	assert(self && *cp);
	return ((*cp)->size);
}
