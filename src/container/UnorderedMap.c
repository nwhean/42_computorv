#include <string.h>

#include "UnorderedMap.h"
#include "UnorderedMap.r"

const void	*UnorderedMap;

/* UnorderedMap constructor method. */
static void	*UnorderedMap_ctor(void *_self, va_list *app)
{
	struct s_UnorderedMap	*self = _self;

	self = super_ctor(UnorderedMap, _self, app);
	self->size = 0;
	self->capacity = 0;
	return (self);
}

/* UnorderedMap copy constructor method */
static void	*UnorderedMap_copy(void *_self)
{
	const struct s_UnorderedMap	*self = _self;
	struct s_UnorderedMap		*object = new(UnorderedMap);
	size_t						i;

	UnorderedMap_reserve(object, self->size);
	for (i = 0; i < self->size; ++i)
		UnorderedMap_insert(object, copy(self->key[i]), copy(self->value[i]));
	return (object);
}

/* UnorderedMap destructor method. */
static void	*UnorderedMap_dtor(void *_self)
{
	struct s_UnorderedMap	*self = _self;

	UnorderedMap_clear(_self);
	free(self->key);
	free(self->value);
	self->key = NULL;
	self->value = NULL;
	return (super_dtor(UnorderedMap, self));
}

/* Checks if the container has no elements */
bool	UnorderedMap_empty(const void *_self)
{
	const struct s_UnorderedMap	*self = _self;

	return (self->size == 0);
}

/* Returns the number of elements in the container */
size_t	UnorderedMap_size(const void *_self)
{
	const struct s_UnorderedMap	*self = _self;

	return (self->size);
}

/* Finds an element with key equivalent to key. */
void	*UnorderedMap_find(const void *_self, const void *key)
{
	const struct s_UnorderedMap	*self = _self;
	size_t						i;

	for (i = 0; i < self->size; ++i)
	{
		if (equal(key, self->key[i]))
			return (self->value[i]);
	}
	return (NULL);
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
	if (self->capacity == self->size)
		UnorderedMap_reserve(self,
							self->capacity == 0 ? 1 : self->capacity * 2);
	self->key[self->size] = (void *)key;
	self->value[self->size] = (void *)value;
	++(self->size);
	return (true);
}

/* Removes specified elements from the container. */
size_t	UnorderedMap_erase(void *_self, const void *key)
{
	struct s_UnorderedMap	*self = _self;
	size_t					i;
	size_t					size;

	for (i = 0; i < self->size; ++i)
	{
		if (equal(key, self->key[i]))
			break ;
	}
	if (i == self->size)
		return (0);
	delete(self->key[i]);
	delete(self->value[i]);
	--(self->size);
	size = self->size * sizeof(void *);
	memmove(&(self->key[i]), &(self->key[i + 1]), size);
	memmove(&(self->value[i]), &(self->value[i + 1]), size);
	self->key[self->size] = NULL;
	self->value[self->size] = NULL;
	return (1);
}

/* Erases all elements from the container.
 *
 * After this call, size() returns zero.
 */
void	UnorderedMap_clear(void *_self)
{
	struct s_UnorderedMap	*self = _self;
	size_t	i;

	for (i = 0; i < self->size; ++i)
	{
		delete(self->key[i]);
		delete(self->value[i]);
	}
	self->size = 0;
}

/* Update capacity to accommodate at least count elements. */
void	UnorderedMap_reserve(void *_self, size_t count)
{
	struct s_UnorderedMap	*self = _self;
	void					**key_new;
	void					**value_new;

	if (self->capacity >= count)
		return ;
	key_new = realloc(self->key, sizeof(void *) * count);
	if (key_new)
		self->key = (void **)key_new;
	else
		return ;
	value_new = realloc(self->value, sizeof(void *) * count);
	if (value_new)
		self->value = value_new;
	else
		return ;
	self->capacity = count;
}

void	initUnorderedMap(void)
{
	if (!UnorderedMap)
		UnorderedMap = new(Class, "UnorderedMap",
				Object, sizeof(struct s_UnorderedMap),
				ctor, UnorderedMap_ctor,
				copy, UnorderedMap_copy,
				dtor, UnorderedMap_dtor,
				0);
}
