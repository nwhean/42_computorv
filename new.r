#include <stdarg.h>
#include <stdlib.h>

struct s_Object
{
	unsigned int	count;
	struct s_Set	*in;
};

struct s_Class
{
	size_t	size;
	void	*(*ctor)(void *self, va_list *app);
	void	*(*dtor)(void *self);
	void	(*draw)(const void *self);
};

struct s_Set {
	unsigned int	count;
};
