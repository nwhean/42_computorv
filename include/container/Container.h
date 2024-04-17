#ifndef CONTAINER_H
# define CONTAINER_H

# include <stdbool.h>
# include <stdlib.h>

# include "Object.h"

struct s_Pair
{
	void	*first;
	void	*second;
};

extern const void	*Container;

extern const void	*ContainerClass;

bool	container_empty(const void *self);
size_t	container_size(const void *self);
void	container_clear(void *self);
bool	container_insert(void *self, void *data);
size_t	container_erase(void *self, void *data);
void	*container_find(const void *self, void *data);
void	container_reserve(void *self, size_t count);

void	initContainer(void);

#endif
