#ifndef UNORDEREDMAP_R
# define UNORDEREDMAP_R

# include <stdbool.h>
# include <stdlib.h>

# include "Object.r"

struct s_UnorderedMap	/* UnorderedMap: Object */
{
	const struct s_Object	_;
	size_t					size;
	size_t					capacity;
	void					**key;
	void					**value;
	int						(*key_cmp)(const void *key1, const void *key2);
};

#endif
