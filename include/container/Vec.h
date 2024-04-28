#ifndef VEC_H
# define VEC_H

# include <stdbool.h>
# include <stdlib.h>

# include "Object.h"

extern const void	*Vec;	/* new(Vec); */

void	initVec(void);

/* capacity */
size_t	Vec_size(const void *self);
size_t	Vec_capacity(const void *self);
bool	Vec_empty(const void *self);
void	Vec_reserve(void *self, size_t n);

/* element access */
void	*Vec_at(const void *self, size_t n);
void	*Vec_front(const void *self);
void	*Vec_back(const void *self);
void	**Vec_data(const void *_self);

/* modifiers */
void	Vec_push_back(void *self, void *value);
void	Vec_pop_back(void *self);
void	Vec_erase(void *self, size_t position);
void	Vec_clear(void *self);

/* non-member function */
void	swap_Vec(void *self, void *other);

#endif
