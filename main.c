#include "Point.h"
#include "Circle.h"

int	main(int argc, char **argv)
{
	(void)argc;
	initPoint();
	initCircle();
	while (*++argv)
	{
		void	*p;
		switch (**argv)
		{
			case 'p':
				p = new(Point, 1, 2);
				break;
			case 'c':
				p = new(Circle, 1, 2, 3);
				break;
			default:
				continue;
		}
		draw(p);
		move(p, 10, 20);
		draw(p);
		delete(p);
	}
	return (0);
}
