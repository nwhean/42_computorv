#include <stdio.h>

#include "new.r"

struct s_Point
{
	const void	*class;
	int			x;
	int			y;
};

static void	*Point_ctor(void *_self, va_list *app)
{
	struct s_Point	*self;

	self = _self;
	self->x = va_arg(*app, int);
	self->y = va_arg(*app, int);
	return (self);
}

static void	Point_draw(const void *_self)
{
	const struct s_Point	*self = _self;

	printf("\".\" at %d,%d\n", self->x, self->y);
}

static const struct s_Class	_Point = {
	sizeof(struct s_Point),
	Point_ctor,
	0,
	Point_draw
};

const void	*Point = &_Point;

void	move(void *_self, int dx, int dy)
{
	struct s_Point	*self;

	self = _self;
	self->x += dx;
	self->y += dy;
}
