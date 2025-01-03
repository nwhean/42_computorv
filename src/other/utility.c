#include <stdio.h>

#include "Object.h"
#include "utility.h"

/* wrapper exit function */
void	*ft_exit(const void *params, void *env)
{
	(void)params;
	(void)env;
	exit(0);
}

/* print out a list of variables and values in the environment */
void	*ft_vars(const void *params, void *env)
{
	char	*s = str(env);

	(void)params;
	printf("%s\n", s);
	free(s);
	return (NULL);
}

/* Exchanges the values of x and y. */
void	swap_double(double *x, double *y)
{
	double	z = *x;

	*x = *y;
	*y = z;
}

/* Exchanges the values of x and y. */
void	swap_unsigned_long(unsigned long *x, unsigned long *y)
{
	unsigned long	z = *x;

	*x = *y;
	*y = z;
}

/* Exchanges the values of x and y. */
void	swap_size_t(size_t *x, size_t *y)
{
	size_t	z = *x;

	*x = *y;
	*y = z;
}

/* Exchanges the content pointed to by of x and y. */
void	swap_ptr(void *_x, void *_y)
{
	void	**x = _x;
	void	**y = _y;
	void	*z = *x;

	*x = *y;
	*y = z;
}
