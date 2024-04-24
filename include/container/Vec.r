#ifndef VEC_R
# define VEC_R

# include <stdbool.h>
# include <stdlib.h>

# include "Object.r"

struct s_Vec	/* Vec: Object */
{
	const struct s_Object	_;
	size_t					size;
	size_t					capacity;
	void					**data;
};

#endif
