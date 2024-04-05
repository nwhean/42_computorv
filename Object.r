#ifndef OBJECT_R
# define OBJECT_R

# include <stdarg.h>
# include <stdlib.h>
# include <stdio.h>

struct s_Object
{
	const struct s_Class	*class;
};

struct s_Class
{
	const struct s_Object	_;
	const char				*name;
	const struct s_Class	*super;
	size_t					size;
	void					*(*ctor)(void *self, va_list *app);
	void					*(*dtor)(void *self);
	int						(*differ)(const void *self, const void *b);
	int						(*puto)(const void *self, FILE *fp);
};

void	*super_ctor(const void *_class, void *_self, va_list *app);
void	*super_dtor (const void *_class, void *_self);

#endif
