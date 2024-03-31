#include <stdio.h>

#include "new.h"
#include "Object.h"
#include "Set.h"

#include "String.h"
#include "new.h"

int	main(void)
{
	void	*a;
	void	*aa;
	void	*b;

	a = new(String, "a");
	aa = clone(a);
	b = new(String, "b");
	printf("sizeOf(a) == %lu\n", sizeOf(a));
	if (differ(a, b))
		puts("ok");
	if (differ(a, aa))
		puts("differ?");
	if (a == aa)
		puts("clone?");
	delete(a);
	delete(aa);
	delete(b);
	return (0);
}
