#ifndef POLYNOMIAL_H
# define POLYNOMIAL_H

# include "Numeric.h"

/* all members are public */
struct s_Polynomial	/* Polynomial: Numeric */
{
	const struct s_Numeric	_;
	void					*word;
	void					*coeffs;	/* Vec*/
};

extern const void	*Polynomial;	/* new(Polynomial, POLYNOMIAL); */

void	initPolynomial(void);

/* capacity */
size_t	Polynomial_size(const void *self);

/* element access */
void	*Polynomial_at(const void *self, size_t n);
void	Polynomial_update(void *self, size_t n, const void *val);

/* Polynomial operations */
void	Polynomial_simplify(void *self);

#endif
