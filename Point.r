#ifndef POINT_R
# define POINT_R

# include "Object.r"

struct s_Point			// Point: Object
{
	const struct s_Object	_;
	int						x;
	int						y;
};

# define x(p)	(((const struct Point *)(p))->x)
# define y(p)	(((const struct Point *)(p))->y)

void	super_draw(const void *class, const void *self);

struct	s_PointClass	// PointClass: Class
{
	const struct s_Class	_;
	void					(*draw)(const void *self);
};

#endif
