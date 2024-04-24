#ifndef OBJECT_R
# define OBJECT_R

# include <stdbool.h>
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
	void					*(*copy)(const void *self);
	void					*(*dtor)(void *self);
	char					*(*str)(const void *self);
	bool					(*equal)(const void *self, const void *other);
	int						(*puto)(const void *self, FILE *fp);
};

void	*super_ctor(const void *_class, void *_self, va_list *app);
void	*super_copy(const void *_class, const void *_self);
void	*super_dtor(const void *_class, void *_self);
char	*super_str(const void *_class, const void *_self);
bool	super_equal(const void *_class, const void *_self, const void *_other);

#endif
