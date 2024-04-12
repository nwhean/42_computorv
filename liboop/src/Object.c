#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include "Object.h"
#include "Object.r"

size_t	sizeOf(const void *_self)
{
	const struct s_Class	*class = classOf(_self);

	return (class->size);
}

const void	*classOf(const void *_self)
{
	const struct s_Object	*self = _self;

	assert(self && self->class);
	return (self->class);
}

const void	*super(const void *_self)
{
	const struct s_Class	*self = _self;

	assert(self && self->super);
	return (self->super);
}

void	*ctor(void *_self, va_list *app)
{
	const struct s_Class	*class = classOf(_self);

	assert(class->ctor);
	return (class->ctor(_self, app));
}

void	*dtor(void *_self)
{
	const struct s_Class	*class = classOf(_self);

	assert(class->dtor);
	return (class->dtor(_self));
}

void	*super_ctor(const void *_class, void *_self, va_list *app)
{
	const struct s_Class	*superclass = super(_class);

	assert(_self && superclass->ctor);
	return (superclass->ctor(_self, app));
}

void	*super_dtor(const void *_class, void *_self)
{
	const struct s_Class	*superclass = super(_class);

	assert(_self && superclass->dtor);
	return (superclass->dtor(_self));
}

void	*new(const void *_class, ...)
{
	const struct s_Class	*class = _class;
	struct s_Object			*object;
	va_list					ap;

	assert(class && class->size);
	object = calloc(1, class->size);
	assert(object);
	object->class = class;
	va_start(ap, _class);
	object = ctor(object, &ap);
	va_end(ap);
	return (object);
}

void	delete(void *_self)
{
	if (_self)
		free(dtor(_self));
}

int	differ(const void *self, const void *b)
{
	const struct s_Class *const	*cp = self;

	if (self && *cp && (*cp)->puto)
		return ((*cp)->differ(self, b));
	return (self == b);
}

int	puto(const void *self, FILE *fp)
{
	const struct s_Class *const	*cp = self;

	if (self && *cp && (*cp)->puto)
		return ((*cp)->puto(self, fp));
	return (-1);
}

static void	*Object_ctor(void *_self, va_list *app)
{
	(void)app;
	return (_self);
}

static void	*Object_dtor(void *_self)
{
	return (_self);
}

static int	Object_differ(const void *_self, const void *b)
{
	return (_self == b);
}

static int	Object_puto(const void *_self, FILE *fp)
{
	const struct s_Class	*class = classOf(_self);

	return (fprintf(fp, "%s at %p\n", class->name, _self));
}

static void	*Class_ctor(void *_self, va_list *app)
{
	struct s_Class	*self;
	const size_t	offset = offsetof(struct s_Class, ctor);

	self = _self;
	self->name = va_arg(*app, char *);
	self->super = va_arg(*app, struct s_Class *);
	self->size = va_arg(*app, size_t);
	assert(self->super);
	memcpy(
		(char *)self + offset,
		(char *)self->super + offset,
		sizeOf(self->super) - offset);
	{
		typedef void	(*voidf)();
		voidf			selector;
		va_list			ap;

		va_copy(ap, *app);
		while ((selector = va_arg(ap, voidf)))
		{
			voidf	method = va_arg(ap, voidf);
			if (selector == (voidf)ctor)
				*(voidf *)&self->ctor = method;
			else if (selector == (voidf)dtor)
				*(voidf *)&self->dtor = method;
			else if (selector == (voidf)differ)
				*(voidf *)&self->differ = method;
			else if (selector == (voidf)puto)
				*(voidf *)&self->puto = method;
		}
		return (self);
	}
}

static void	*Class_dtor(void *_self)
{
	const struct s_Class	*self = _self;

	fprintf(stderr, "%s: cannot destroy class\n", self->name);
	return (0);
}

static const struct s_Class	object[] = {
	{
		{object + 1},
		"Object", object, sizeof(struct s_Object),
		Object_ctor, Object_dtor, Object_differ, Object_puto
	},
	{
		{object + 1},
		"Class", object, sizeof(struct s_Class),
		Class_ctor, Class_dtor, Object_differ, Object_puto
	}
};

const void					*Object = object;
const void					*Class = object + 1;
