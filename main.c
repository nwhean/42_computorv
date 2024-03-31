#include <stdio.h>

#include "new.h"
#include "Object.h"
#include "Set.h"

const void	*g_Set;
const void	*g_Object;

int	main(void)
{
	void	*s;
	void	*a;
	void	*b;
	void	*c;

	s = new(g_Set);
	a = add(s, new(g_Object));
	b = add(s, new(g_Object));
	c = new(g_Object);
	if (contains(s, a) && contains(s, b))
		puts("ok");
	if (contains(s, c))
		puts("contains?");
	if (differ(a, add(s, a)))
		puts("differs?");
	delete(drop(s, b));
	delete(drop(s, c));
	return (0);
}
