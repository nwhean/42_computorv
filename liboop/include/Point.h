#ifndef POINT_H
# define POINT_H

# include "Object.h"

extern const void	*Point;	/* new(Point, x, y); */
void	move(void *point, int dx, int dy);

extern const void	*PointClass;	/* adds draw */
void	draw(const void *self);

void	initPoint(void);

#endif
