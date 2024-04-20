#include <string.h>

#include "UnorderedMap.h"
#include "UnorderedMap.r"

const void	*UnorderedMap;

/* UnorderedMap constructor method. */
static void	*UnorderedMap_ctor(void *_self, va_list *app)
{
	struct s_UnorderedMap	*self = _self;

	self = super_ctor(UnorderedMap, _self, app);
	self->key_cmp = va_arg(*app, int (*)(const void *, const void *));
	self->size = 0;
	self->capacity = 0;
	return (self);
}

/* UnorderedMap destructor method. */
static void	*UnorderedMap_dtor(void *_self)
{
	struct s_UnorderedMap	*self = _self;

	UnorderedMap_clear(_self);
	return (self);
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
		delete((void *)self->key[i]);
		delete(self->value[i]);
	}
	free(self->key);
	free(self->value);
	self->key = NULL;
	self->value = NULL;
	self->size = 0;
}

/* Inserts a key, value pair into the container, if the container doesn't
 * already contain an element with an equivalent key.
 */
bool	UnorderedMap_insert(void *_self, const void *key, void *value)
{
	struct s_UnorderedMap	*self = _self;
	void					*found = UnorderedMap_find(_self, key);

	if (found)
		return (false);
	if (self->capacity == self->size)
		UnorderedMap_reserve(self,
							self->capacity == 0 ? 1 : self->capacity * 2);
	self->key[self->size] = key;
	self->value[self->size] = value;
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
		if (self->key_cmp(key, self->key[i]) == 0)
			break ;
	}
	if (i == self->size)
		return (0);
	--(self->size);
	size = self->size * sizeof(void *);
	memmove(&(self->key[i]), &(self->key[i + 1]), size);
	memmove(&(self->value[i]), &(self->value[i + 1]), size);
	self->key[self->size] = NULL;
	self->value[self->size] = NULL;
	return (1);
}

/* Finds an element with key equivalent to key. */
void	*UnorderedMap_find(const void *_self, const void *key)
{
	const struct s_UnorderedMap	*self = _self;
	size_t						i;

	for (i = 0; i < self->size; ++i)
	{
		if (self->key_cmp(key, self->key[i]) == 0)
			return (self->value[i]);
	}
	return (NULL);
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
		self->key = (const void **)key_new;
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
				dtor, UnorderedMap_dtor,
				0);
}
