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

/* default constructor */
void	*ctor(void *_self, va_list *app)
{
	const struct s_Class	*class = classOf(_self);

	assert(class->ctor);
	return (class->ctor(_self, app));
}

void	*super_ctor(const void *_class, void *_self, va_list *app)
{
	const struct s_Class	*superclass = super(_class);

	assert(_self && superclass->ctor);
	return (superclass->ctor(_self, app));
}

/* copy constructor */
void	*copy(const void *_self)
{
	const struct s_Class	*class = classOf(_self);

	assert(class->copy);
	return (class->copy(_self));
}

void	*super_copy(const void *_class, const void *_self)
{
	const struct s_Class	*superclass = super(_class);

	assert(_self && superclass->copy);
	return (superclass->copy(_self));
}

/* destructor */
void	*dtor(void *_self)
{
	const struct s_Class	*class = classOf(_self);

	assert(class->dtor);
	return (class->dtor(_self));
}

void	*super_dtor(const void *_class, void *_self)
{
	const struct s_Class	*superclass = super(_class);

	assert(_self && superclass->dtor);
	return (superclass->dtor(_self));
}

/* Return a string representation of an object. */
char	*str(const void *_self)
{
	const struct s_Class	*class = classOf(_self);

	assert(class->str);
	return (class->str(_self));
}

char	*super_str(const void *_class, const void *_self)
{
	const struct s_Class	*superclass = super(_class);

	assert(_self && superclass->str);
	return (superclass->str(_self));
}

/* Compare two objects. */
int	differ(const void *self, const void *b)
{
	const struct s_Class *const	*cp = self;

	if (self && *cp && (*cp)->puto)
		return ((*cp)->differ(self, b));
	return (self == b);
}

/* Display object to a FILE stream. */
int	puto(const void *self, FILE *fp)
{
	const struct s_Class *const	*cp = self;

	if (self && *cp && (*cp)->puto)
		return ((*cp)->puto(self, fp));
	return (-1);
}

/* Object default constructor. */
static void	*Object_ctor(void *_self, va_list *app)
{
	(void)app;
	return (_self);
}

/* Object copy constructor. */
static void	*Object_copy(const void *_self)
{
	const struct s_Class	*class = classOf(_self);
	struct s_Object			*object;

	assert(class && class->size);
	object = calloc(1, class->size);
	assert(object);
	object->class = class;
	return (object);
}

/* Object destructor. */
static void	*Object_dtor(void *_self)
{
	return (_self);
}

/* Object string representation. */
static char	*Object_str(const void *_self)
{
	(void)_self;
	return (NULL);
}

/* Return 1 if the inputs are equal, 0 otherwise. */
static int	Object_differ(const void *_self, const void *b)
{
	return (_self == b);
}

/* Print information of the object to the FILE stream. */
static int	Object_puto(const void *_self, FILE *fp)
{
	const struct s_Class	*class = classOf(_self);

	return (fprintf(fp, "%s at %p\n", class->name, _self));
}

/* Class default constructor. */
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
			if (selector == (voidf)ctor)
				*(voidf *)&self->ctor = method;
			else if (selector == (voidf)copy)
				*(voidf *)&self->copy = method;
			else if (selector == (voidf)dtor)
				*(voidf *)&self->dtor = method;
			else if (selector == (voidf)str)
				*(voidf *)&self->str = method;
			else if (selector == (voidf)differ)
				*(voidf *)&self->differ = method;
			else if (selector == (voidf)puto)
				*(voidf *)&self->puto = method;
			#pragma GCC diagnostic pop
		}
		return (self);
	}
}

/* Class copy constructor. */
static void	*Class_copy(const void *_self)
{
	const struct s_Class	*self = _self;

	fprintf(stderr, "%s: cannot copy class\n", self->name);
	return (NULL);
}

/* Class destructor. */
static void	*Class_dtor(void *_self)
{
	const struct s_Class	*self = _self;

	fprintf(stderr, "%s: cannot destroy class\n", self->name);
	return (NULL);
}

static const struct s_Class	object[] = {
	{
		{object + 1},
		"Object", object, sizeof(struct s_Object),
		Object_ctor, Object_copy, Object_dtor, Object_str,
		Object_differ, Object_puto
	},
	{
		{object + 1},
		"Class", object, sizeof(struct s_Class),
		Class_ctor, Class_copy, Class_dtor, Object_str,
		Object_differ, Object_puto
	}
};

const void					*Object = object;
const void					*Class = object + 1;
