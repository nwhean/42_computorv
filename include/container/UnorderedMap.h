#ifndef UNORDEREDMAP_H
# define UNORDEREDMAP_H

# include <stdbool.h>
# include <stdlib.h>

# include "Container.h"

extern const void	*UnorderedMap;	/* new(UnorderedMap, key_equal); */

extern const void	*UnorderedMapClass;

void	initUnorderedMap(void);

#endif
