#ifndef CONTAINER_R
# define CONTAINER_R

# include <stdbool.h>
# include <stdlib.h>

# include "Object.r"

struct s_Container	/* Container: Object */
{
	const struct s_Object	_;
};

struct	s_ContainerClass	/* ContainerClass: Class */
{
	const struct s_Class	_;
	bool					(*empty)(const void *self);
	size_t					(*size)(const void *self);
	void					(*clear)(void *self);
	bool					(*insert)(void *self, void *data);
	size_t					(*erase)(void *self, void *data);
	void					*(*find)(const void *self, void *data);
	void					(*reserve)(void *self, size_t count);
};

#endif
