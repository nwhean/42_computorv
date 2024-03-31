#ifndef SET_H
# define SET_H

struct s_Set {
	unsigned	count;
};

extern const void	*Set;

void	*add(void *set, const void *element);
void	*find(const void *set, const void *element);
void	*drop(void *set, const void *element);
int		contains(const void *set, const void *element);

#endif
