#ifndef OBJECT_H
# define OBJECT_H

# include <stdarg.h>
# include <stdio.h>

extern const void	*Object;

size_t		sizeOf(const void *self);
const void	*classOf(const void *self);
const void	*super(const void *self);
void		*new(const void *_class, ...);	/* new(Object); */
void		delete(void *self);

/* instance method */
void		*ctor(void *self, va_list *app);	/* default constructor */
void		*copy(const void *self);			/* copy constructor */
void		*dtor(void *self);					/* destructor */
char		*str(const void *self);				/* convert to char * */
int			differ(const void *self, const void *b);
int			puto(const void *self, FILE *fp);

extern const void	*Class;
/* new(Cls, "name", super, size, selector, method, ..., 0) */

#endif
