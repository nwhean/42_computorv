#include <stdio.h>

#include "new.h"
#include "new.r"
#include "Circle.h"
#include "Circle.r"

static void	*Circle_ctor(void *_self, va_list *app)
{
	struct s_Circle	*self;

	self = ((const struct s_Class *)Point)->ctor(_self, app);
	self->rad = va_arg(*app, int);
	return (self);
}

static void	Circle_draw(const void *_self)
{
	const struct s_Circle	*self = _self;

	printf("circle at %d,%d rad %d\n", self->_.x, self->_.y, self->rad);
}

static const struct s_Class	_Circle = {
	sizeof(struct s_Circle),
	Circle_ctor,
	0,
	Circle_draw
};

const void					*Circle = &_Circle;
