#ifndef OBJECT_H
# define OBJECT_H

# include <stdarg.h>
# include <stdio.h>

extern const void	*Object;

size_t		sizeOf(const void *self);
const void	*classOf(const void *self);
const void	*super(const void *self);
void		*ctor(void *self, va_list *app);
void		*dtor(void *self);

void		*new(const void *_class, ...);	// new(Object);
void		delete(void *self);
int			differ(const void *self, const void *b);
int			puto(const void *self, FILE *fp);

extern const void	*Class;	// new(Cls, "name", super, size, sel, meth, ..., 0)

#endif
