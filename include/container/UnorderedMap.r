#ifndef UNORDEREDMAP_R
# define UNORDEREDMAP_R

# include <stdbool.h>
# include <stdlib.h>

# include "Object.r"

struct s_UnorderedMap	/* UnorderedMap: Object */
{
	const struct s_Object	_;
	void					*key;
	void					*value;
};

#endif
