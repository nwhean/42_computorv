#ifndef STR_H
# define STR_H

# include <stdbool.h>
# include <stdlib.h>

# include "Object.h"

extern const void	*Str;	/* new(Str, char *); */

/* capacity */
size_t		Str_size(const void *self);
size_t		Str_capacity(const void *self);
void		Str_reserve(void *self, size_t n);
void		Str_clear(void *self);
bool		Str_empty(const void *self);
void		*Str_append(void *self, const char *s);
void		Str_push_back(void *self, char c);
const char*	Str_c_str(const void *self);
int			Str_compare(const void *self, const void *other);

void	initStr(void);
char	*strdup(const char *s);

#endif
