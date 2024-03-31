#include <assert.h>
#include <stdlib.h>

void	*new(const void *type, ...)
{
	const size_t	size = *(const size_t *)type;
	void			*p;

	p = calloc(1, size);
	assert(p);
	return (p);
}

void	delete(void *_item)
{
	free(_item);
}
