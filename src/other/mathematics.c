/* container */
#include "Vec.h"

/* inter */
#include "Expr.h"

/* lexer */
#include "Rational.h"

/* other */
#include "mathematics.h"
#include "mathematics.r"

size_t	max(size_t a, size_t b)
{
	return (a > b ? a : b);
}

/* Return the exponentiation of a Rational number */
struct s_Rational	*ft_exp_Rational(struct s_Rational *x)
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
			x->denominator <<= 1;
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

/* Return the exponentiation of a Rational number */
struct s_Rational	*ft_ln_Rational(struct s_Rational *_x)
{
	struct s_Rational	*one;
	struct s_Rational	*n;
	struct s_Rational	*sum;
	struct s_Rational	*x;
	struct s_Rational	*x1;
	struct s_Rational	*term = NULL;
	struct s_Rational	*ln2;
	struct s_Rational	*multiplier;
	long				i = 0;

	if (_x->numerator <= 0)
	{
		fprintf(stderr, "ft_ln_Rational: domain error.\n");
		return (NULL);
	}

	/* scale until 0 < x < 1 */
	x = copy(_x);
	while (x->numerator / x->denominator > 1)
	{
		if (x->denominator > 1e9)
			x->numerator >>= 1;
		else
			x->denominator <<= 1;
		++i;
	}

	one = new(Rational, RATIONAL, 1, 1);
	n = new(Rational, RATIONAL, 1, 1);

	/* ln(x) = -(x-1) - (x-1)^2 / 2 - (x-1)^3 / 3 - (x-1)^4 / 4 + ... */
	sum = new(Rational, RATIONAL, 0, 1);	/* sum = 0 */
	numeric_isub((void **)&x, one);			/* x = -(x - 1) */
	numeric_ineg((void **)&x);
	x1 = copy(x);							/* x1 = x  */
	do {
		delete(term);
		term = numeric_div(x1, n);			/* term = x1 / n */
		numeric_isub((void **)&sum, term);	/* sum -= term */
		numeric_iadd((void **)&n, one);		/* n += 1*/
		numeric_imul((void **)&x1, x);		/* x1 = x1 * x  */
	} while (x1->numerator != 0 && term->denominator < 1e9);
	delete(term);

	/* sum = sum + i * ln(2) */
	ln2 = Rational_from_double(0.6931471805599453);
	multiplier = new(Rational, RATIONAL, i, 1);
	numeric_imul((void **)&ln2, multiplier);
	numeric_iadd((void **)&sum, ln2);

	delete(one);
	delete(n);
	delete(x);
	delete(x1);
	delete(ln2);
	delete(multiplier);
	return (sum);
}

/* Return the exponentiation of a number. */
void	*ft_ln(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_ln_Rational(x);
			break ;
		default:
			fprintf(stderr, "ln function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the sin of a Rational number */
struct s_Rational	*ft_sin_Rational(struct s_Rational *_x)
{
	struct s_Rational	*x = copy(_x);
	struct s_Rational	*cos;
	struct s_Rational	*sin;
	struct s_Rational	*one;
	struct s_Rational	*n;
	struct s_Rational	*sum;
	struct s_Rational	*x1;
	struct s_Rational	*term;
	int					i = 0;

	/* scale until -1 < x < 1 */
	if (x->numerator > x->denominator
			|| (x->numerator < 0 && -x->numerator > x->denominator))
	{
		if (x->denominator > 1e9)
			x->numerator >>= 1;
		else
			x->denominator <<= 1;

		cos = ft_cos_Rational(x);
		sin = ft_sin_Rational(x);
		sum = numeric_mul(cos, sin);

		if (sum->denominator > 1e9)
			sum->numerator <<= 1;
		else
			sum->denominator >>= 1;

		delete(cos);
		delete(sin);
		delete(x);
		return (sum);
	}

	one = new(Rational, RATIONAL, 1, 1);
	n = new(Rational, RATIONAL, 0, 1);
	/* sin(x) =  x - x^3 / 3! + x^5 / 5! - x^7 / 7! + ... */
	sum = new(Rational, RATIONAL, 0, 1);	/* sum = 0 */
	x1 = new(Rational, RATIONAL, 1, 1);		/* x1 = 1 */
	do {
		if (i % 2)
		{
			if ((i + 1) % 4)
				numeric_iadd((void **)&sum, x1);	/* sum += x1 */
			else
				numeric_isub((void **)&sum, x1);	/* sum -= x1 */
		}
		numeric_iadd((void **)&n, one);		/* n += 1*/
		term = numeric_div(x, n);			/* term = x / n */
		numeric_imul((void **)&x1, term);	/* x1 = x1 * x/n */
		delete(term);
		++i;
	} while (x1->numerator != 0);

	delete(one);
	delete(n);
	delete(x);
	delete(x1);
	return (sum);
}

/* Return the sin of a number. */
void	*ft_sin(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_sin_Rational(x);
			break ;
		default:
			fprintf(stderr, "sin function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the cos of a Rational number */
struct s_Rational	*ft_cos_Rational(struct s_Rational *_x)
{
	struct s_Rational	*x = copy(_x);
	struct s_Rational	*cos;
	struct s_Rational	*sin;
	struct s_Rational	*one;
	struct s_Rational	*n;
	struct s_Rational	*sum;
	struct s_Rational	*x1;
	struct s_Rational	*term;
	int					i = 0;

	/* scale until -1 < x < 1 */
	if (x->numerator > x->denominator
			|| (x->numerator < 0 && -x->numerator > x->denominator))
	{
		if (x->denominator > 1e9)
			x->numerator >>= 1;
		else
			x->denominator <<= 1;

		cos = ft_cos_Rational(x);
		numeric_imul((void **)&cos, cos);
		sin = ft_sin_Rational(x);
		numeric_imul((void **)&sin, sin);
		sum = numeric_sub(cos, sin);

		delete(cos);
		delete(sin);
		delete(x);
		return (sum);
	}

	one = new(Rational, RATIONAL, 1, 1);
	n = new(Rational, RATIONAL, 0, 1);
	/* sin(x) = 1 - x^2 / 2! + x^4 / 4! - x^6 / 6! + ... */
	sum = new(Rational, RATIONAL, 0, 1);	/* sum = 0 */
	x1 = new(Rational, RATIONAL, 1, 1);		/* x1 = 1 */
	do {
		if (i % 2 == 0)
		{
			if (i % 4)
				numeric_isub((void **)&sum, x1);	/* sum -= x1 */
			else
				numeric_iadd((void **)&sum, x1);	/* sum += x1 */
		}
		numeric_iadd((void **)&n, one);		/* n += 1*/
		term = numeric_div(x, n);			/* term = x / n */
		numeric_imul((void **)&x1, term);	/* x1 = x1 * x/n */
		delete(term);
		++i;
	} while (x1->numerator != 0);

	delete(one);
	delete(n);
	delete(x);
	delete(x1);
	return (sum);
}

/* Return the cos of a number. */
void	*ft_cos(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_cos_Rational(x);
			break ;
		default:
			fprintf(stderr, "tan function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the tan of a Rational number */
struct s_Rational	*ft_tan_Rational(struct s_Rational *x)
{
	struct s_Rational	*sin = ft_sin_Rational(x);
	struct s_Rational	*cos = ft_cos_Rational(x);
	struct s_Rational	*retval = numeric_div(sin, cos);

	delete(sin);
	delete(cos);
	return (retval);
}

/* Return the tan of a number. */
void	*ft_tan(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_tan_Rational(x);
			break ;
		default:
			fprintf(stderr, "cos function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}

/* Return the square root of a Rational number */
struct s_Rational	*ft_sqrt_Rational(struct s_Rational *x)
{
	struct s_Rational	*half = Rational_from_double(0.5);
	struct s_Rational	*retval = numeric_pow(x, half);

	delete(half);
	return (retval);
}

/* Return the tan of a number. */
void	*ft_sqrt(const void *params, void *env)
{
	void		*x = eval(Vec_at(params, 0), env);
	enum e_Tag	tag= Token_get_tag(x);
	void		*retval = NULL;

	switch (tag)
	{
		case RATIONAL:
			retval = ft_sqrt_Rational(x);
			break ;
		default:
			fprintf(stderr, "sqrt function is not defined for input type.\n");
	}

	delete(x);
	return (retval);
}
