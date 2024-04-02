#ifndef NEW_H
# define NEW_H

# include <stdlib.h>

void	*new(const void *_class, ...);
void	delete(void *self);
void	draw(const void *self);
size_t	sizeOf(const void *self);

#endif
