#include <stdarg.h>

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
	void	*(*clone)(const void *self);
	int		(*differ)(const void *self, const void *b);
};

struct s_String {
	const void	*class;
	char		*text;
};

struct s_Set {
	unsigned int	count;
};
