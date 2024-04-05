#include "Object.h"

static int	Any_differ(const void *_self, const void *b)
{
	(void)_self;
	(void)b;
	return (0);
}

int	main(void)
{
	void	*o = new(Object);
	void	*Any = new(
					Class, "Any", Object, sizeOf(o),
					differ, Any_differ,
					0);
	void	*a = new(Any);

	puto(Any, stdout);
	puto(o, stdout);
	puto(a, stdout);
	if (differ(o, o) == differ(a, a))
		puts("ok");
	if (differ(o, a) != differ(a, o))
		puts("not commutative");
	delete(o);
	delete(a);
	delete(Any);
	return (0);
}
