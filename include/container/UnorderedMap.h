#ifndef UNORDEREDMAP_H
# define UNORDEREDMAP_H

# include <stdbool.h>
# include <stdlib.h>

# include "Object.h"

extern const void	*UnorderedMap;	/* new(UnorderedMap); */

void	initUnorderedMap(void);

/* capacity */
bool	UnorderedMap_empty(const void *self);
size_t	UnorderedMap_size(const void *self);

/* element lookup */
void	*UnorderedMap_find(const void *self, const void *key);
void	*UnorderedMap_keys(const void *self);
void	*UnorderedMap_values(const void *self);

/* modifiers */
bool	UnorderedMap_insert(void *self, const void *key, const void *value);
size_t	UnorderedMap_erase(void *self, const void *key);
void	UnorderedMap_clear(void *self);

/* hash policy */
void	UnorderedMap_reserve(void *self, size_t count);

/* non-member function */
void	swap_UnorderedMap(void *self, void *other);

#endif
