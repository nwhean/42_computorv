#include <assert.h>

#include "Container.h"
#include "Container.r"

const void	*Container;
const void	*ContainerClass;

/* Container constructor method. */
static void	*Container_ctor(void *_self, va_list *app)
{
	struct s_Container	*self;

	self = super_ctor(Container, _self, app);
	return (self);
}

bool	container_empty(const void *self)
{
	const struct s_ContainerClass *const	*cp = self;

	assert(self && *cp && (*cp)->empty);
	return ((*cp)->empty(self));
}

size_t	container_size(const void *self)
{
	const struct s_ContainerClass *const	*cp = self;

	assert(self && *cp && (*cp)->size);
	return ((*cp)->size(self));
}

void	container_clear(void *self)
{
	const struct s_ContainerClass	**cp = self;

	assert(self && *cp && (*cp)->clear);
	(*cp)->clear(self);
}

bool	container_insert(void *self, void *data)
{
	const struct s_ContainerClass	**cp = self;

	assert(self && *cp && (*cp)->insert);
	return ((*cp)->insert(self, data));
}

size_t	container_erase(void *self, void *data)
{
	const struct s_ContainerClass	**cp = self;

	assert(self && *cp && (*cp)->erase);
	return ((*cp)->erase(self, data));
}

void	*container_find(const void *self, void *data)
{
	const struct s_ContainerClass *const	*cp = self;

	assert(self && *cp && (*cp)->find);
	return ((*cp)->find(self, data));
}

void	container_reserve(void *self, size_t count)
{
	const struct s_ContainerClass *const	*cp = self;

	assert(self && *cp && (*cp)->reserve);
	(*cp)->reserve(self, count);
}

/* ContainerClass constructor method. */
static void	*ContainerClass_ctor(void *_self, va_list *app)
{
	typedef void			(*voidf)();
	struct s_ContainerClass	*self;
	voidf					selector;
	va_list					ap;

	self = super_ctor(ContainerClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		method = va_arg(ap, voidf);
		if (selector == (voidf)container_empty)
			*(voidf *)&self->empty = method;
		else if (selector == (voidf)container_size)
			*(voidf *)&self->size = method;
		else if (selector == (voidf)container_clear)
			*(voidf *)&self->clear = method;
		else if (selector == (voidf)container_insert)
			*(voidf *)&self->insert = method;
		else if (selector == (voidf)container_erase)
			*(voidf *)&self->erase = method;
		else if (selector == (voidf)container_find)
			*(voidf *)&self->find = method;
		else if (selector == (voidf)container_reserve)
			*(voidf *)&self->reserve = method;
		#pragma GCC diagnostic pop;
	}
	return (self);
}

void	initContainer(void)
{
	if (!ContainerClass)
		ContainerClass = new(Class, "ContainerClass",
				Class, sizeof(struct s_ContainerClass),
				ctor, ContainerClass_ctor,
				0);
	if (!Container)
		Container = new(ContainerClass, "Container",
				Object, sizeof(struct s_Container),
				ctor, Container_ctor,
				0);
}
