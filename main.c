#include <stdio.h>

#include "new.h"
#include "Object.h"
#include "Set.h"

int	main(void)
{
	static const size_t	_Set = sizeof(struct s_Set);
	static const size_t	_Object = sizeof(struct s_Object);
	const void			*Set = &_Set;
	const void			*Object = &_Object;
	void				*s;
	void				*a;
	void				*b;
	void				*c;

	s = new(Set);
	a = add(s, new(Object));
	b = add(s, new(Object));
	c = new(Object);
	if (contains(s, a) && contains(s, b))
		puts("ok");
	if (contains(s, c))
		puts("contains?");
	if (differ(a, add(s, a)))
		puts("differs?");
	delete(drop(s, a));
	delete(drop(s, b));
	delete(c);
	delete(s);
	return (0);
}
