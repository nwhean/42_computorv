#include <string.h>

#include "UnorderedMap.h"
#include "UnorderedMap.r"
#include "Vec.h"

const void	*UnorderedMap;

/* UnorderedMap constructor method. */
static void	*UnorderedMap_ctor(void *_self, va_list *app)
{
	struct s_UnorderedMap	*self = _self;

	self = super_ctor(UnorderedMap, _self, app);
	self->key = new(Vec);
	self->value = new(Vec);
	return (self);
}

/* UnorderedMap copy constructor method */
static void	*UnorderedMap_copy(void *_self)
{
	const struct s_UnorderedMap	*self = _self;
	struct s_UnorderedMap		*object = new(UnorderedMap);

	delete(object->key);
	delete(object->value);
	object->key = copy(self->key);
	object->value = copy(self->value);
	return (object);
}

/* UnorderedMap destructor method. */
static void	*UnorderedMap_dtor(void *_self)
{
	struct s_UnorderedMap	*self = _self;

	delete(self->key);
	delete(self->value);
	return (super_dtor(UnorderedMap, self));
}

/* Checks if the container has no elements */
bool	UnorderedMap_empty(const void *_self)
{
	const struct s_UnorderedMap	*self = _self;

	return (UnorderedMap_size(self) == 0);
}

/* Returns the number of elements in the container */
size_t	UnorderedMap_size(const void *_self)
{
	const struct s_UnorderedMap	*self = _self;

	return (Vec_size(self->key));
}

/* Finds an element with key equivalent to key. */
void	*UnorderedMap_find(const void *_self, const void *key)
{
	const struct s_UnorderedMap	*self = _self;
	size_t						size = UnorderedMap_size(self);
	size_t						i;

	for (i = 0; i < size; ++i)
	{
		if (equal(key, Vec_at(self->key, i)))
			return (Vec_at(self->value, i));
	}
	return (NULL);
}

/* Return a vector containing the keys to the UnorderedMap */
void	*UnorderedMap_keys(const void *_self)
{
	const struct s_UnorderedMap	*self = _self;

	return (self->key);
}

/* Return a vector containing the values to the UnorderedMap */
void	*UnorderedMap_values(const void *_self)
{
	const struct s_UnorderedMap	*self = _self;

	return (self->value);
}

/* Inserts a key, value pair into the container, if the container doesn't
 * already contain an element with an equivalent key.
 */
bool	UnorderedMap_insert(void *_self, const void *key, const void *value)
{
	struct s_UnorderedMap	*self = _self;
	void					*found = UnorderedMap_find(_self, key);

	if (found)
		return (false);
	Vec_push_back(self->key, (void *)key);
	Vec_push_back(self->value, (void *)value);
	return (true);
}

/* Removes specified elements from the container. */
size_t	UnorderedMap_erase(void *_self, const void *key)
{
	struct s_UnorderedMap	*self = _self;
	size_t					size = UnorderedMap_size(self);
	size_t					i;

	for (i = 0; i < size; ++i)
	{
		if (equal(key, Vec_at(self->key, i)))
			break ;
	}
	if (i == size)
		return (0);
	Vec_erase(self->key, i);
	Vec_erase(self->value, i);
	return (1);
}

/* Erases all elements from the container.
 *
 * After this call, size() returns zero.
 */
void	UnorderedMap_clear(void *_self)
{
	struct s_UnorderedMap	*self = _self;

	Vec_clear(self->key);
	Vec_clear(self->value);
}

/* Update capacity to accommodate at least count elements. */
void	UnorderedMap_reserve(void *_self, size_t count)
{
	struct s_UnorderedMap	*self = _self;

	Vec_reserve(self->key, count);
	Vec_reserve(self->value, count);
}

/* The contents of container `self` are exchanged with those of `other`.
 */
void	swap_UnorderedMap(void *_self, void *_other)
{
	struct s_UnorderedMap	*self = _self;
	struct s_UnorderedMap	*other = _other;

	swap_Vec(self->key, other->key);
	swap_Vec(self->value, other->value);
}

void	initUnorderedMap(void)
{
	initVec();
	if (!UnorderedMap)
		UnorderedMap = new(Class, "UnorderedMap",
				Object, sizeof(struct s_UnorderedMap),
				ctor, UnorderedMap_ctor,
				copy, UnorderedMap_copy,
				dtor, UnorderedMap_dtor,
				0);
}
