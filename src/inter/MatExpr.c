#include <assert.h>
#include <stdarg.h>

/* container */
#include "Str.h"
#include "Vec.h"

/* inter */
#include "Expr.h"
#include "Expr.r"
#include "MatExpr.h"
#include "VecExpr.h"

/* lexer */
#include "Rational.h"
#include "Matrix.h"

/* other */
#include "mathematics.h"

const void	*MatExpr;

/* MatExpr constructor method. */
static void	*MatExpr_ctor(void *_self, va_list *app)
{
	struct s_MatExpr	*self;
	void				*vec;
	size_t				i;
	size_t				j;

	self = super_ctor(MatExpr, _self, app);
	vec = va_arg(*app, void *);	/* a Vec containing VecExpr */
	self->rows = Vec_size(vec);
	self->cols = 0;
	self->vec = new(Vec);

	for (i = 0; i < self->rows; ++i)
	{
		struct s_VecExpr	*vexpr = Vec_at(vec, i);

		/* calculate maximum column size among the rows */
		self->cols = max(self->cols, Vec_size(vexpr->vec));

		/* push the Vec containing Expr into self->vec */
		Vec_push_back(self->vec, vexpr->vec);
		vexpr->vec = NULL;
	}

	/* ensure all rows are of the same size */
	for (i = 0; i < self->rows; ++i)
	{
		void	*vec_i = Vec_at(self->vec, i);

		/* convert self->vec into a Vec containing Vecs containing Expr */
		for (j = Vec_size(vec_i); j < self->cols; ++j)
			Vec_push_back(vec_i, new(Rational, RATIONAL, 0, 1));
	}

	delete(vec);
	return (self);
}

/* MatExpr destructor method. */
static void	*MatExpr_dtor(void *_self)
{
	struct s_MatExpr	*self = _self;

	delete(self->vec);
	return (super_dtor(MatExpr, _self));
}

static char	*MatExpr_str(const void *_self)
{
	const struct s_MatExpr	*self = _self;
	void					*s = new(Str, "[");
	char					*retval;
	size_t					i;

	for (i = 0; i < self->rows; ++i)
	{
		char	*s_append;

		s_append = str(Vec_at(self->vec, i));
		Str_append(s, s_append);
		free(s_append);
		if (i < self->rows - 1)
			Str_append(s, "; ");
	}
	Str_push_back(s, ']');
	retval = str(s);
	delete(s);
	return (retval);
}

/* Evaluate the expression into a Matrix class */
static struct s_Token	*MatExpr_eval(const void *_self)
{
	const struct s_MatExpr	*self = _self;
	void					*vec = new(Vec);
	size_t					i;

	for (i = 0; i < self->rows; ++i)
	{
		void	*vec_r = new(Vec);
		void	*vec_i = Vec_at(self->vec, i);
		size_t	j;

		for (j = 0; j < self->cols; ++j)
			Vec_push_back(vec_r, eval(Vec_at(vec_i, j)));
		Vec_push_back(vec, vec_r);
	}
	return (new(Matrix, MATRIX, vec));
}

void	initMatExpr(void)
{
	initStr();
	initVec();
	initExpr();
	initRational();
	initMatrix();
	if (!MatExpr)
		MatExpr = new(ExprClass, "MatExpr",
				Expr, sizeof(struct s_MatExpr),
				ctor, MatExpr_ctor,
				dtor, MatExpr_dtor,
				str, MatExpr_str,
				eval, MatExpr_eval,
				0);
}
