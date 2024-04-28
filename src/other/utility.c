#include "utility.h"

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
void	swap_ptr(void **x, void **y)
{
	void	*z = *x;

	*x = *y;
	*y = z;
}
