#ifndef STR_R
# define STR_R

# include <stdbool.h>
# include <stdlib.h>

# include "Object.r"

struct s_Str	/* String: Object */
{
	const struct s_Object	_;
	size_t	size;
	size_t	capacity;
	char	*buffer;
};

#endif
