/* container */
#include "Vec.h"

/* inter */
#include "Expr.h"

/* lexer */
#include "Rational.h"

/* other */
#include "mathematics.h"

size_t	max(size_t a, size_t b)
{
	return (a > b ? a : b);
}

/* Return the exponentiation of a Rational number */
static struct s_Rational	*ft_exp_Rational(struct s_Rational *x)
{
	struct s_Rational	*one = new(Rational, RATIONAL, 1, 1);
	struct s_Rational	*n = new(Rational, RATIONAL, 0, 1);
	struct s_Rational	*sum;
	struct s_Rational	*x1;
	struct s_Rational	*term;
	int					i = 0;

	/* scale until -1 < x < 1 */
	while (x->numerator > x->denominator
			|| (x->numerator < 0 && -x->numerator > x->denominator))
	{
		if (x->denominator > 1e9)
			x->numerator >>= 1;
		else
			x->denominator *= 2;
		++i;
	}

	/* exp(x) = 1 + x + x^2 / 2! + x^3 / 3! + x^4 / 4! + ... */
	sum = new(Rational, RATIONAL, 0, 1);	/* sum = 0 */
	x1 = new(Rational, RATIONAL, 1, 1);		/* x1 = 1 */
	do {
		numeric_iadd((void **)&sum, x1);	/* sum += x1 */
		numeric_iadd((void **)&n, one);		/* n += 1*/
		term = numeric_div(x, n);			/* term = x / n */
		numeric_imul((void **)&x1, term);	/* x1 = x1 * x/n */
		delete(term);
	} while (x1->numerator != 0);

	/* square until i = 0 */
	for (; i > 0; --i)
	{
		void	*cpy = copy(sum);

		numeric_imul((void **)&sum, cpy);
		delete(cpy);
	}

	delete(one);
	delete(n);
	delete(x1);
	return (sum);
}

/* Return the exponentiation of a number. */
void	*ft_exp(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_exp_Rational(x);
			break ;
		default:
			fprintf(stderr, "exp function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}
