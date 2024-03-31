#include <assert.h>
#include <stdlib.h>

#include "new.r"
#include "Set.h"

/*
 * Increments the element's reference counter and the number of elements
 * in the set.
 */
void	*add(void *_set, const void *_element)
{
	struct s_Set	*set;
	struct s_Object	*element;

	set = _set;
	element = (void *)_element;
	assert(set);
	assert(element);
	if (!element->in)
		element->in = set;
	else
		assert(element->in == set);
	++element->count;
	++set->count;
	return (element);
}

/*
 * Looks if its `element` contains the proper index for the set.
 */
void	*find(const void *_set, const void *_element)
{
	const struct s_Object	*element = _element;

	assert(_set);
	assert(element);
	if (element->in == _set)
		return ((void *)element);
	else
		return (NULL);
}

/*
 * Removes the `element` from the `set`.
 */
void	*drop(void *_set, const void *_element)
{
	struct s_Set	*set;
	struct s_Object	*element;

	set = _set;
	element = find(set, _element);
	if (element)
	{
		if (--element->count == 0)
			element->in = 0;
		--set->count;
	}
	return (element);
}

int	contains(const void *_set, const void *_element)
{
	return (find(_set, _element) != NULL);
}
