#include <assert.h>
#include <stddef.h>

#if !defined MANY || MANY < 1
# define MANY	10
#endif

static int	g_heap[MANY];

void	*new(const void *type, ...)
{
	int	*p;

	(void)type;
	p = g_heap + 1;
	while (p < g_heap + MANY)
	{
		if (!*p)
			break ;
		++p;
	}
	assert(p < g_heap + MANY);
	*p = MANY;
	return (p);
}

void	delete(void *_item)
{
	int	*item;

	item = _item;
	if (item)
	{
		assert(item > g_heap && item < g_heap + MANY);
		*item = 0;
	}
}

/*
 * If an `element` contains MANY, it can be added to the `set`,
 * otherwise, it should already be in the `set` because we do not permit
 * an object to belong to more than one `set`.
 */
void	*add(void *_set, const void *_element)
{
	int			*set;
	const int	*element = _element;

	set = _set;
	assert(set > g_heap && set < g_heap + MANY);
	assert(*set == MANY);
	assert(element > g_heap && element < g_heap + MANY);
	if (*element == MANY)
		*(int *)element = set - g_heap;
	else
		assert(*element == set - g_heap);
	return ((void *)element);
}

/*
 * Looks if its `element` contains the proper index for the set.
 */
void	*find(const void *_set, const void *_element)
{
	const int	*set;
	const int	*element = _element;

	set = _set;
	assert(set > g_heap && set < g_heap + MANY);
	assert(*set == MANY);
	assert(element > g_heap && element < g_heap + MANY);
	assert(*element);
	if (*element == set - g_heap)
		return ((void *)element);
	else
		return (NULL);
}

/*
 * Removes the `element` from the `set`.
 */
void	*drop(void *_set, const void *_element)
{
	int	*element;

	element = find(_set, _element);
	if (element)
		*element = MANY;
	return (element);
}

int	contains(const void *_set, const void *_element)
{
	return (find(_set, _element) != NULL);
}
