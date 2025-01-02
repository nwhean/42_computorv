/* container */
#include "Str.h"
#include "Vec.h"

/* lexer */
#include "Complex.h"
#include "Rational.h"
#include "Polynomial.h"
#include "Word.h"

const void	*Polynomial;

/* Polynomial constructor method. */
static void	*Polynomial_ctor(void *_self, va_list *app)
{
	struct s_Polynomial	*self = _self;

	self = super_ctor(Polynomial, _self, app);
	self->word = NULL;
	self->coeffs = new(Vec);
	return (self);
}

/* Return a copy of the Polynomial. */
static struct s_Polynomial	*Polynomial_copy(const void *_self)
{
	const struct s_Polynomial	*self = _self;
	struct s_Polynomial			*retval;

	retval = super_copy(Polynomial, self);
	retval->word = self->word ? copy(self->word) : NULL;
	retval->coeffs = copy(self->coeffs);
	if (retval->coeffs == NULL)
	{
		delete(retval);
		return (NULL);
	}
	return (retval);
}

/* Polynomial destructor method. */
static void	*Polynomial_dtor(void *_self)
{
	struct s_Polynomial	*self = _self;

	delete(self->word);
	delete(self->coeffs);
	return(super_dtor(Polynomial, _self));
}

/* Return string representing the Polynomial. */
static char	*Polynomial_str(const void *_self)
{
	const struct s_Polynomial	*self = _self;
	void						*s = new(Str, "Polynomial(");	/* Str */
	char						*s_append;
	char						*retval;
	size_t						i;

	for (i = 0; i < Polynomial_size(self); ++i)
	{
		s_append = str(Vec_at(self->coeffs, i));
		Str_append(s, s_append);
		free(s_append);
		if (i < Polynomial_size(self) - 1)
			Str_append(s, ", ");
	}
	Str_push_back(s, ')');
	retval = str(s);
	delete(s);
	return (retval);
}

/* Return true if two Polynomials are the same, false otherwise. */
static bool	Polynomial_equal(const void *_self, const void *_other)
{
	const struct s_Polynomial	*self = _self;
	const struct s_Polynomial	*other = _other;

	if (!super_equal(Polynomial, _self, _other))
		return (false);

	/* do not need to compare the id */

	return (equal(self->coeffs, other->coeffs));
}

/* Return the add / sub / mul / div of Polynomial with Scalar. */
static void	*Polynomial_add_sub_Scalar(const void *self,
									const void *other,
									void *(*func)(const void *, const void *))
{
	struct s_Polynomial	*retval = copy(self);

	Polynomial_update(retval, 0, func(Vec_at(retval->coeffs, 0), other));
	Polynomial_simplify(retval);
	return (retval);
}

/* Return the addition or subtraction of Polynomial with Polynomial. */
static void	*Polynomial_add_sub_Polynomial(const void *_self,
									const void *_other,
									void *(*func)(const void *, const void *))
{
	const struct s_Polynomial	*self = _self;
	const struct s_Polynomial	*other = _other;
	struct s_Polynomial			*retval = new(Polynomial, POLYNOMIAL);
	size_t						size_self = Polynomial_size(self);
	size_t						size_other = Polynomial_size(other);
	size_t						n;
	size_t						i;
	void						*lhs;
	void						*rhs;

	if (self->word && other->word && !equal(self->word, other->word))
	{
		fprintf(stderr, "%s\n",
				"Polynomial_add_sub_Polynomial: different variables.");
		return (NULL);
	}

	n = size_self > size_other ? size_self : size_other;
	Vec_reserve(retval->coeffs, n);
	for (i = 0; i < n; i++)
	{
		lhs = Polynomial_at(self, i);
		rhs = Polynomial_at(other, i);
		if (i < size_self && i < size_other)
			Polynomial_update(retval, i, func(lhs, rhs));
		else if (i < size_self)
			Polynomial_update(retval, i, lhs);
		else
			Polynomial_update(retval, i, rhs);
	}
	Polynomial_simplify(retval);
	return (retval);
}

/* Return the addition of two Numerics. */
static void	*Polynomial_add(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return (Polynomial_add_sub_Scalar(_self, _other, numeric_add));
		case VECTOR:
			fprintf(stderr, "%s\n",
					"Polynomial_add: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n",
					"Polynomial_add: incompatible with Matrix.");
			return (NULL);
		case POLYNOMIAL:
			return (Polynomial_add_sub_Polynomial(_self, _other, numeric_add));
		default:
			fprintf(stderr, "%s\n", "Polynomial_add: unexpected input type.");
			return (NULL);
	};
}

/* Return the subtraction of two Numerics. */
static void	*Polynomial_sub(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return (Polynomial_add_sub_Scalar(_self, _other, numeric_sub));
		case VECTOR:
			fprintf(stderr, "%s\n",
					"Polynomial_sub: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n",
					"Polynomial_sub: incompatible with Matrix.");
			return (NULL);
		case POLYNOMIAL:
			return (Polynomial_add_sub_Polynomial(_self, _other, numeric_sub));
		default:
			fprintf(stderr, "%s\n", "Polynomial_sub: unexpected input type.");
			return (NULL);
	};
}

/* Return the addition or subtraction of Polynomial with Scalar. */
static void	*Polynomial_mul_div_Scalar(const void *_self,
									const void *other,
									void *(*func)(const void *, const void *))
{
	const struct s_Polynomial	*self = _self;
	struct s_Polynomial			*retval = new(Polynomial, POLYNOMIAL);
	size_t						i;

	for (i = 0; i < Polynomial_size(self); i++)
		Polynomial_update(retval, i, func(Polynomial_at(self, i), other));
	Polynomial_simplify(retval);
	return (retval);
}

/* Return the multiplication of two Numerics. */
static void	*Polynomial_mul(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return (Polynomial_mul_div_Scalar(_self, _other, numeric_mul));
		case VECTOR:
			fprintf(stderr, "%s\n",
					"Polynomial_mul: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n",
					"Polynomial_mul: incompatible with Matrix.");
			return (NULL);
		case POLYNOMIAL:
			fprintf(stderr, "%s\n",
					"Polynomial_mul: incompatible with Polynomial.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Polynomial_mul: unexpected input type.");
			return (NULL);
	};
}

/* Return the division of two Numerics. */
static void	*Polynomial_div(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return (Polynomial_mul_div_Scalar(_self, _other, numeric_div));
		case VECTOR:
			fprintf(stderr, "%s\n",
					"Polynomial_div: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n",
					"Polynomial_div: incompatible with Matrix.");
			return (NULL);
		case POLYNOMIAL:
			fprintf(stderr, "%s\n",
					"Polynomial_div: incompatible with Polynomial.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Polynomial_div: unexpected input type.");
			return (NULL);
	};
}

/* Return remainder from the division of one Numeric from another. */
static void	*Polynomial_mod(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
		case COMPLEX:
			return Polynomial_mul_div_Scalar(_self, _other, numeric_mod);
		case VECTOR:
			fprintf(stderr, "%s\n",
					"Polynomial_mod: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n",
					"Polynomial_mod: incompatible with Matrix.");
			return (NULL);
		case POLYNOMIAL:
			fprintf(stderr, "%s\n",
					"Polynomial_mod: incompatible with Polynomial.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Polynomial_mod: unexpected input type.");
			return (NULL);
	};
}

/* Return a copy of the Polynomial with its value negated. */
static struct s_Polynomial	*Polynomial_neg(const void *_self)
{
	const struct s_Polynomial	*self = _self;
	struct s_Polynomial			*retval = new(Polynomial, POLYNOMIAL);
	void						*value;
	size_t						i;

	for (i = 0; i < Polynomial_size(self); ++i)
	{
		value = copy(Polynomial_at(self, i));
		numeric_ineg(&value);
		Polynomial_update(retval, i, value);
	}
	return (retval);
}

void	*Polynomial_pow_Rational(const void *_self, const void *_other)
{
	const struct s_Polynomial	*self = _self;
	const struct s_Rational		*other = _other;
	struct s_Polynomial			*retval;
	size_t						size = Polynomial_size(self);
	size_t						i;

	/* ensure that all degrees are 0, other than i = size - 1 */
	for (i = 0; i < size - 1; i++)
	{
		if (!numeric_iszero(Polynomial_at(self, i)))
		{
			fprintf(stderr, "%s\n",
					"Polynomial_pow: does not support multiple coefficients");
			return (NULL);
		}
	}

	if (size == 1 && numeric_iszero(other))
	{
		fprintf(stderr, "%s\n", "0^0 is undefined.");
		return (NULL);
	}

	if (!numeric_isinteger(other))
	{
		fprintf(stderr, "%s\n", "Polynomial_pow: exponent must be integral.");
		return (NULL);
	}

	retval = new(Polynomial, POLYNOMIAL);
	retval->word = self->word ? copy(self->word) : NULL;
	Polynomial_update(retval, (size - 1)*(size - 1),
					numeric_pow(Polynomial_at(self, i), other));
	return (retval);
}

/* Return the exponentiation of one Numeric to another. */
static void	*Polynomial_pow(const void *_self, const void *_other)
{
	switch (Token_get_tag(_other))
	{
		case RATIONAL:
			return (Polynomial_pow_Rational(_self, _other));
		case COMPLEX:
			fprintf(stderr, "%s\n",
					"Polynomial_pow: incompatible with Complex.");
			return (NULL);
		case VECTOR:
			fprintf(stderr, "%s\n",
					"Polynomial_pow: incompatible with Vector.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n",
					"Polynomial_pow: incompatible with Matrix.");
			return (NULL);
		case POLYNOMIAL:
			fprintf(stderr, "%s\n",
					"Polynomial_pow: incompatible with Polynomial.");
			return (NULL);
		default:
			fprintf(stderr, "%s\n", "Polynomial_pow: unexpected input type.");
			return (NULL);
	};
}

/* Promote one Numeric type to another */
static void	*Polynomial_promote(const void *_self, enum e_Tag tag)
{
	const struct s_Polynomial	*self = _self;

	switch (tag)
	{
		case RATIONAL:
		case COMPLEX:
			fprintf(stderr, "%s\n", "Polynomial_promotion: "
					"cannot demote Polynomial to scalar type.");
			return (NULL);
		case VECTOR:
			fprintf(stderr, "%s\n", "Polynomial_promotion: "
					"cannot promote Polynomial to Vector type.");
			return (NULL);
		case MATRIX:
			fprintf(stderr, "%s\n", "Polynomial_promotion: "
					"cannot promote Polynomial to Matrix type.");
			return (NULL);
		case POLYNOMIAL:
			return Polynomial_copy(self);
		default:
			fprintf(stderr, "%s\n","Polynomial_promotion: "
					"unexpected input type.");
			return (NULL);
	};
}

/* Return true if all entries of the Polynomial is zero. */
static bool	Polynomial_iszero(const void *_self)
{
	const struct s_Vector	*self = _self;
	size_t	i;

	for (i = 0; i < Polynomial_size(self); ++i)
	{
		if (!numeric_iszero(Polynomial_at(self, i)))
			return (false);
	}
	return (true);
}

void	initPolynomial(void)
{
	initNumeric();
	if (!Polynomial)
	{
		Polynomial = new(NumericClass, "Polynomial",
					Numeric, sizeof(struct s_Polynomial),
					ctor, Polynomial_ctor,
					copy, Polynomial_copy,
					dtor, Polynomial_dtor,
					str, Polynomial_str,
					equal, Polynomial_equal,
					numeric_add, Polynomial_add,
					numeric_sub, Polynomial_sub,
					numeric_mul, Polynomial_mul,
					numeric_div, Polynomial_div,
					numeric_mod, Polynomial_mod,
					numeric_neg, Polynomial_neg,
					numeric_pow, Polynomial_pow,
					numeric_promote, Polynomial_promote,
					numeric_iszero, Polynomial_iszero,
					0);
		initStr();
		initVec();
		initRational();
		initComplex();
		initWord();
	}
}

/* Return the size of the Polynomial. */
size_t	Polynomial_size(const void *_self)
{
	const struct s_Polynomial	*self = _self;

	return (Vec_size(self->coeffs));
}

/* Returns a reference to the element at position `n` in the Polynomial.
 *
 * Returns NULL if `n` is out of range.
 */
void	*Polynomial_at(const void *_self, size_t n)
{
	const struct s_Polynomial	*self = _self;

	if (n < Polynomial_size(self))
		return (Vec_at(self->coeffs, n));
	return (NULL);
}

/* Update the coefficient of index 'n' with value 'val' */
void	Polynomial_update(void *_self, size_t n, const void *_val)
{
	struct s_Polynomial	*self = _self;
	struct s_Vec		*vec = self->coeffs;
	void				*val = (void *)_val;
	void				**data;

	if (n < Polynomial_size(self))	/* update an existing term of polynomial */
	{
		data = Vec_data(vec);
		delete(data[n]);
		data[n] = val;
	}
	else							/* extend the polynomial with new terms */
	{
		n = (n - Polynomial_size(self));
		while (n--)
			Vec_push_back(vec, Rational_from_long(0, 1));
		Vec_push_back(vec, val);
	}
}

/* Remove higher order terms with 0 as coefficients. */
void	Polynomial_simplify(void *_self)
{
	struct s_Polynomial	*self = _self;
	size_t 				order = -1;
	size_t				i;

	/* detect the highest order of a polynomial */
	for (i = 0; i < Polynomial_size(self); i++)
	{
		if (!numeric_iszero(Polynomial_at(self, i)))
			order = i;
	}

	if (order == 0 && numeric_iszero(Polynomial_at(self, 0)))
		Vec_clear(self->coeffs);	/* all terms are zero */
	else
	{
		/* remove terms larger than 'order' */
		order = Polynomial_size(self) - order;
		while (--order)
			Vec_pop_back(self->coeffs);
	}
}
