#ifndef UNORDEREDMAP_H
# define UNORDEREDMAP_H

# include <stdbool.h>
# include <stdlib.h>

# include "Object.h"

extern const void	*UnorderedMap;	/* new(UnorderedMap, key_equal); */

void	initUnorderedMap(void);
bool	UnorderedMap_empty(const void *self);
size_t	UnorderedMap_size(const void *self);
void	UnorderedMap_clear(void *self);
bool	UnorderedMap_insert(void *self, void *key, void *value);
size_t	UnorderedMap_erase(void *self, void *key);
void	*UnorderedMap_find(const void *self, void *key);
void	UnorderedMap_reserve(void *self, size_t count);

#endif
