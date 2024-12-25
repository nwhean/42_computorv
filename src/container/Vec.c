#include <string.h>

/* containter */
#include "Str.h"
#include "Vec.h"
#include "Vec.r"

/* other */
#include "utility.h"

const void	*Vec;

/* Vec constructor method. */
static void	*Vec_ctor(void *_self, va_list *app)
{
	struct s_Vec	*self = _self;

	self = super_ctor(Vec, _self, app);
	self->size = 0;
	self->capacity = 0;
	return (self);
}

/* Vec copy constructor method. */
static void	*Vec_copy(const void *_self)
{
	const struct s_Vec	*self = _self;
	struct s_Vec		*object = new(Vec);
	size_t				i;

	Vec_reserve(object, self->size);
	for (i = 0; i < self->size; ++i)
		Vec_push_back(object, copy(self->data[i]));
	return (object);
}

/* Vec destructor method. */
static void	*Vec_dtor(void *_self)
{
	struct s_Vec	*self = _self;

	Vec_clear(_self);
	free(self->data);
	self->data = NULL;
	return (super_dtor(Vec, self));
}

/* Return the string representation of the vector. */
char	*Vec_str(const void *_self)
{
	const struct s_Vec	*self = _self;
	void				*s = new(Str, "[");	/* Str */
	char				*s_append;
	char				*retval;
	size_t				i;

	for (i = 0; i < self->size; ++i)
	{
		s_append = str(Vec_at(self, i));
		Str_append(s, s_append);
		free(s_append);
		if (i < self->size - 1)
			Str_append(s, ", ");
	}
	Str_push_back(s, ']');
	retval = str(s);
	delete(s);
	return (retval);
}

/* Returns the number of elements in the vector. */
size_t	Vec_size(const void *_self)
{
	const struct s_Vec	*self = _self;

	return (self->size);
}

/* Returns the equality of two vectors. */
static bool	Vec_equal(const void *_self, const void *_other)
{
	const struct s_Vec	*self = _self;
	const struct s_Vec	*other = _other;
	size_t				i;

	if (!super_equal(Vec, _self, _other))
		return (false);
	if (self->size != other->size)
		return (false);
	for (i = 0; i < self->size; ++i)
	{
		if (!equal(self->data[i], other->data[i]))
			return (false);
	}
	return (true);
}

/* Returns the size of the storage space currently allocated for the
 * vector, expressed in terms of elements.
 */
size_t	Vec_capacity(const void *_self)
{
	const struct s_Vec	*self = _self;

	return (self->capacity);
}

/* Returns whether the vector is empty (i.e. whether its size is 0). */
bool	Vec_empty(const void *_self)
{
	const struct s_Vec	*self = _self;

	return (self->size == 0);
}

/* Requests that the vector capacity be at least enough to contain n
 * elements.
 */
void	Vec_reserve(void *_self, size_t n)
{
	struct s_Vec	*self = _self;
	void			**data_new;

	if (self->capacity >= n)
		return ;
	data_new = realloc(self->data, sizeof(void *) * n);
	if (data_new)
		self->data = data_new;
	else
		return ;
	self->capacity = n;
}

/* Returns a reference to the element at position `n` in the vector.
 *
 * Returns NULL if `n` is out of range.
 */
void	*Vec_at(const void *_self, size_t n)
{
	const struct s_Vec	*self = _self;

	if (n > self->size)
		return (NULL);
	return ((void *)(self->data[n]));
}

/* Returns a reference to the first element in the vector. */
void	*Vec_front(const void *_self)
{
	return (Vec_at(_self, 0));
}

/* Returns a reference to the last element in the vector.. */
void	*Vec_back(const void *_self)
{
	return (Vec_at(_self, Vec_size(_self) - 1));
}

/* Returns a direct pointer to the memory array used internally by the
 * vector to store its owned elements. */
void	**Vec_data(const void *_self)
{
	const struct s_Vec	*self = _self;

	return (self->data);
}

/* Adds a new element at the end of the vector, after its current last element.
 */
void	Vec_push_back(void *_self, void *value)
{
	struct s_Vec	*self = _self;

	if (self->capacity == self->size)
		Vec_reserve(self, self->capacity == 0 ? 1 : self->capacity * 2);
	self->data[self->size] = value;
	++(self->size);
}

/* Removes the last element in the vector, effectively reducing the
 * container size by one. */
void	Vec_pop_back(void *_self)
{
	struct s_Vec	*self = _self;

	/* undefined behaviour if self->size == 0 */
	delete(self->data[--(self->size)]);
}

/* Removes from the vector either a single element (position). */
void	Vec_erase(void *_self, size_t position)
{
	struct s_Vec	*self = _self;
	size_t			size;

	/* invalid position causes undefined behavior */
	delete(self->data[position]);
	if (self->size > position)
	{
		size = (self->size - position - 1) * sizeof(void *);
		memmove(&(self->data[position]), &(self->data[position + 1]), size);
	}
	self->data[--(self->size)] = NULL;
}

/* Removes all elements from the vector (which are destroyed), leaving
 * the container with a size of 0.
 */
void	Vec_clear(void *_self)
{
	struct s_Vec	*self = _self;
	size_t			i;

	for (i = 0; i < self->size; ++i)
		delete((void *)self->data[i]);
	self->size = 0;
}

/* The contents of container x are exchanged with those of y. */
void	swap_Vec(void *_self, void *_other)
{
	struct s_Vec	*self = _self;
	struct s_Vec	*other = _other;

	swap_size_t(&self->size, &other->size);
	swap_size_t(&self->capacity, &other->capacity);
	swap_ptr(&self->data, &other->data);
}

void	initVec(void)
{
	initStr();
	if (!Vec)
		Vec = new(Class, "Vec",
				Object, sizeof(struct s_Vec),
				ctor, Vec_ctor,
				copy, Vec_copy,
				dtor, Vec_dtor,
				str, Vec_str,
				equal, Vec_equal,
				0);
}
