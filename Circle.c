#include <stdio.h>

#include "Circle.h"
#include "Circle.r"

/* Circle constructor method. */
static void	*Circle_ctor(void *_self, va_list *app)
{
	struct s_Circle	*self;

	self = super_ctor(Circle, _self, app);
	self->rad = va_arg(*app, int);
	return (self);
}

/* Circle draw method. */
static void	Circle_draw(const void *_self)
{
	const struct s_Circle	*self = _self;

	printf("circle at %d,%d rad %d\n", self->_.x, self->_.y, self->rad);
}

const void	*Circle;

void	initCircle(void)
{
	if (!Circle)
	{
		initPoint();
		Circle = new(PointClass, "Circle",
				Point, sizeof(struct s_Circle),
				ctor, Circle_ctor,
				draw, Circle_draw,
				0);
	}
}
