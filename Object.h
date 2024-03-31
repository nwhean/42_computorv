#ifndef OBJECT_H
# define OBJECT_H

struct s_Object
{
	unsigned		count;
	struct s_Set	*in;
};

extern const void	*Object;

int	differ(const void *a, const void *b);

#endif
