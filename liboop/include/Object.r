#ifndef OBJECT_R
# define OBJECT_R

# include <stdarg.h>
# include <stdlib.h>
# include <stdio.h>

struct s_Object
{
	const struct s_Class	*class;	/* object’s description */
};

struct s_Class
{
	const struct s_Object	_;		/* class’ description */
	const char				*name;	/* class’ name */
	const struct s_Class	*super;	/* class’ super class */
	size_t					size;	/* class’ object’s size */
	void					*(*ctor)(void *self, va_list *app);
	void					*(*dtor)(void *self);
	int						(*differ)(const void *self, const void *b);
	int						(*puto)(const void *self, FILE *fp);
};

void	*super_ctor(const void *_class, void *_self, va_list *app);
void	*super_dtor (const void *_class, void *_self);

#endif