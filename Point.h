#ifndef POINT_H
# define POINT_H

# include "Object.h"

extern const void	*Point;

void	draw(const void *self);
void	move(void *point, int dx, int dy);

extern const void	*PointClass;

void	initPoint(void);

#endif
