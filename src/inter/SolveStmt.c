/* container */
#include "Str.h"

/* lexer */
#include "Polynomial.h"
#include "Rational.h"

/* inter */
#include "Arith.h"
#include "Expr.h"
#include "SolveStmt.h"

/* other */
#include "mathematics.r"

const void	*SolveStmt;

/* SolveStmt constructor method. */
static void	*SolveStmt_ctor(void *_self, va_list *app)
{
	struct s_SolveStmt	*self;

	self = super_ctor(SolveStmt, _self, app);
	self->lhs = va_arg(*app, struct s_Expr *);
	self->rhs = va_arg(*app, struct s_Expr *);

	if (self->lhs && self->rhs)
		return (self);
	if (!self->lhs)
		fprintf(stderr, "SolveStmt_ctor: LHS is ill-defined.\n");
	if (!self->rhs)
		fprintf(stderr, "SolveStmt_ctor: RHS is ill-defined.\n");
	delete(self);
	return (NULL);
}

/* SolveStmt destructor method. */
static void	*SolveStmt_dtor(void *_self)
{
	struct s_SolveStmt	*self = _self;

	delete(self->lhs);
	delete(self->rhs);
	return (super_dtor(SolveStmt, _self));
}

static char	*SolveStmt_str(const void *_self)
{
	const struct s_SolveStmt	*self = _self;
	void						*s = new(Str, "");
	char						*s_append;
	char						*retval;

	s_append = str(self->lhs);
	Str_append(s, s_append);
	free(s_append);
	Str_append(s, " = ");
	s_append = str(self->rhs);
	Str_append(s, s_append);
	free(s_append);
	retval = str(s);
	delete(s);
	return (retval);
}

/* Put the solve the equation */
static void	SolveStmt_exec(void *_self, void *env)
{
	struct s_SolveStmt	*self = _self;
	struct s_Arith		*arith;
	void				*net;
	size_t				size;
	void				*a = NULL;
	void				*b = NULL;
	void				*c = NULL;
	void				*b_2a = NULL;
	void				*b2 = NULL;
	void				*ac4 = NULL;
	void				*two = Rational_from_long(2, 1);
	void				*four = Rational_from_long(4, 1);
	void				*discriminant = NULL;
	void				*discriminant_sqrt = NULL;
	void				*ans1 = NULL;
	void				*ans2 = NULL;
	char				*s = NULL;
	char				*s1 = NULL;
	char				*s2 = NULL;

	#ifdef DEBUG
	/* print input string */
	s = str(_self);
	printf("%s\n", s);
	free(s);
	#endif

	arith = new(Arith, new(Token, '-'), ZERO, copy(self->lhs), copy(self->rhs));
	net = to_polynomial(arith, env);
	size = Polynomial_size(net);

	if (size == 0)
		fprintf(stderr, "LHS = RHS.\n");
	else if (size == 1)
		fprintf(stderr, "No solution is possible.\n");
	else if (size == 2)
	{
		b = copy(Polynomial_at(net, 1));
		c = copy(Polynomial_at(net, 0));
		ans1 = numeric_neg(c);
		numeric_idiv(&ans1, b);
		s = str(((struct s_Polynomial *)net)->word);
		s1 = str(ans1);
		printf("Solution: %s = %s\n", s, s1);
	}
	else if (size == 3)
	{
		a = copy(Polynomial_at(net, 2));
		b = copy(Polynomial_at(net, 1));
		c = copy(Polynomial_at(net, 0));
		b_2a = numeric_neg(b);
		numeric_idiv(&b_2a, a);
		numeric_idiv(&b_2a, two);
		b2 = numeric_mul(b, b);
		ac4 = numeric_mul(a, c);
		numeric_imul(&ac4, four);
		discriminant = numeric_sub(b2, ac4);
		s = str(((struct s_Polynomial *)net)->word);
		if (numeric_iszero(discriminant))
		{
			s1 = str(b_2a);
			printf("Solution: %s = %s\n", s, s1);
		}
		else
		{
			discriminant_sqrt = ft_sqrt_Rational(discriminant);
			numeric_idiv(&discriminant_sqrt, a);
			numeric_idiv(&discriminant_sqrt, two);
			ans1 = numeric_add(b_2a, discriminant_sqrt);
			ans2 = numeric_sub(b_2a, discriminant_sqrt);
			s1 = str(ans1);
			s2 = str(ans2);
			printf("Solutions: %s = %s; %s = %s\n", s, s1, s, s2);
		};
	}
	if (size > 3)
		fprintf(stderr,
				"Solution to degree %ld polynomial is not supported.\n",
				size - 1);

	delete(arith);
	delete(net);
	delete(a);
	delete(b);
	delete(c);
	delete(b_2a);
	delete(b2);
	delete(ac4);
	delete(two);
	delete(four);
	delete(discriminant);
	delete(discriminant_sqrt);
	delete(ans1);
	delete(ans2);
	free(s);
	free(s1);
	free(s2);
}

void	initSolveStmt(void)
{
	initStr();
	initStmt();
	if (!SolveStmt)
		SolveStmt = new(StmtClass, "SolveStmt",
				Stmt, sizeof(struct s_SolveStmt),
				ctor, SolveStmt_ctor,
				dtor, SolveStmt_dtor,
				str, SolveStmt_str,
				exec, SolveStmt_exec,
				0);
}
