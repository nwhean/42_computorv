#include <stdarg.h>
#include <string.h>

/* container */
#include "Str.h"
#include "Str.r"

/* other */
#include "utility.h"

const void	*Str;

/* Str constructor method.
 * A copy of the string is made, which will be freed upon destruction.
 */
static void	*Str_ctor(void *_self, va_list *app)
{
	struct s_Str	*self = _self;

	self = super_ctor(Str, _self, app);
	self->buffer = strdup(va_arg(*app, char *));
	self->size = strlen(self->buffer);
	self->capacity = self->size + 1;
	return (self);
}

/* Str destructor method. */
static void	*Str_dtor(void *_self)
{
	struct s_Str	*self = _self;

	free(self->buffer);
	return (self);
}

/* Str destructor method. */
void	*Str_copy(const void *_self)
{
	const struct s_Str	*self = _self;

	return new(Str, self->buffer);
}

/* Returns the length of the string, in terms of bytes. */
size_t	Str_size(const void *_self)
{
	const struct s_Str	*self = _self;

	return (self->size);
}

/* Returns the size of the storage space currently allocated for the
 * string, expressed in terms of bytes.
 */
size_t	Str_capacity(const void *_self)
{
	const struct s_Str	*self = _self;

	return (self->capacity);
}

/* Requests that the string capacity be adapted to a planned change in
 * size to a length of up to `n` characters. */
void	Str_reserve(void *_self, size_t n)
{
	struct s_Str	*self = _self;
	char			*temp;

	n += 1;
	if (self->capacity >= n)
		return ;
	temp = realloc(self->buffer, n);
	if (temp)
		self->buffer = temp;
	self->capacity = n;
}

/* Erases the contents of the string, which becomes an empty string. */
void	Str_clear(void *_self)
{
	struct s_Str	*self = _self;

	self->buffer[0] = '\0';
	self->size = 0;
}

/* Returns whether the string is empty (i.e. whether its length is 0). */
bool	Str_empty(const void *_self)
{
	const struct s_Str	*self = _self;

	return (self->size == 0);
}

/* Extends the string by appending additional characters at the end of
 * its current value.
 */
void	*Str_append(void *_self, const char *s)
{
	struct s_Str	*self = _self;
	size_t			size_s = strlen(s);
	size_t			size_new = self->size + size_s;

	Str_reserve(self, size_new);
	memmove(self->buffer + self->size, s, size_s + 1);
	self->size = size_new;
	return (self);
}

/* Appends character c to the end of the string, increasing its length
 * by one. */
void	Str_push_back(void *_self, char c)
{
	struct s_Str	*self = _self;

	if (self->capacity == self->size + 1)
		Str_reserve(_self, self->capacity * 2);
	self->buffer[self->size] = c;
	self->size += 1;
	self->buffer[self->size] = '\0';
}

/* Returns a pointer to an array that contains a null-terminated sequence
 * of characters (i.e., a C-string) representing the current value of the
 * string object.*/
char	*Str_str(const void *_self)
{
	const struct s_Str	*self = _self;

	return (strdup(self->buffer));
}

/* Compares the value of the string object (or a substring) to the
 * sequence of characters specified by its arguments. */
bool	Str_equal(const void *_self, const void *_other)
{
	const struct s_Str	*self = _self;
	const struct s_Str	*other = _other;
	bool				retval;

	retval = super_equal(Str, _self, _other);
	if (retval)
		return (strcmp(self->buffer, other->buffer) == 0);
	return (false);
}

/* Exchanges the content of the Str containers. */
void	swap_Str(void *_self, void *_other)
{
	struct s_Str	*self = _self;
	struct s_Str	*other = _other;

	swap_size_t(&self->size, &other->size);
	swap_size_t(&self->capacity, &other->capacity);
	swap_ptr((void **)&self->buffer, (void **)&other->buffer);
}

void	initStr(void)
{
	if (!Str)
		Str = new(Class, "Str",
				Object, sizeof(struct s_Str),
				ctor, Str_ctor,
				copy, Str_copy,
				dtor, Str_dtor,
				str, Str_str,
				equal, Str_equal,
				0);
}

/* Return a pointer to a new string which is a duplicate of the string `s`.*/
char	*strdup(const char *s)
{
	size_t	len = strlen(s);
	char	*retval = malloc(len + 1);

	memcpy(retval, s, len + 1);
	return (retval);
}
