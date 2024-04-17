#ifndef UNORDEREDMAP_R
# define UNORDEREDMAP_R

# include <stdbool.h>
# include <stdlib.h>

# include "Container.r"

struct s_UnorderedMap	/* UnorderedMap: Container */
{
	const struct s_Container	_;
	size_t						size;
	size_t						capacity;
	void						**key;
	void						**value;
	bool						(*key_equal)(void *key1, void *key2);
};

struct	s_UnorderedMapClass	/* UnorderedMapClass: ContainerClass */
{
	const struct s_ContainerClass	_;
};

#endif
