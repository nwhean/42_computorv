#include <assert.h>
#include <stdio.h>

#include "Point.r"
#include "Point.h"

void	move(void *_self, int dx, int dy)
{
	struct s_Point	*self;

	self = _self;
	self->x += dx;
	self->y += dy;
}

/* Point constructor method. */
static void	*Point_ctor(void *_self, va_list *app)
{
	struct s_Point	*self;

	self = super_ctor(Point, _self, app);
	self->x = va_arg(*app, int);
	self->y = va_arg(*app, int);
	return (self);
}

void	draw(const void *self)
{
	const struct s_PointClass *const	*cp = self;

	assert(self && *cp && (*cp)->draw);
	(*cp)->draw(self);
}

void	super_draw(const void *_class, const void *_self)
{
	const struct s_PointClass	*superclass = super(_class);

	assert(_self && superclass->draw);
	superclass->draw(_self);
}

/* Point draw method. */
static void	Point_draw(const void *_self)
{
	const struct s_Point	*self = _self;

	printf("\".\" at %d,%d\n", self->x, self->y);
}

/* PointClass constructor method. */
static void	*PointClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_PointClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(PointClass, _self, app);
	va_copy(ap, *app);
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		method = va_arg(ap, voidf);
		if (selector == (voidf)draw)
			*(voidf *)&self->draw = method;
	}
	return (self);
}

const void	*Point;
const void	*PointClass;

void	initPoint(void)
{
	if (!PointClass)
		PointClass = new(Class, "PointClass",
				Class, sizeof(struct s_PointClass),
				ctor, PointClass_ctor,
				0);
	if (!Point)
		Point = new(PointClass, "Point",
				Object, sizeof(struct s_Point),
				ctor, Point_ctor,
				draw, Point_draw,
				0);
}
