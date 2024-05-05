#include <assert.h>
#include <stdarg.h>

/* inter */
#include "Expr.h"
#include "Expr.r"
#include "Id.h"
#include "Word.h"

/* symbols */
#include "Env.h"

const void	*Id;

/* Id constructor method. */
static void	*Id_ctor(void *_self, va_list *app)
{
	struct s_Id	*self;

	self = super_ctor(Id, _self, app);
	return (self);
}

/* Id copy constructor method. */
static void	*Id_copy(const void *_self)
{
	return (super_copy(Id, _self));
}

/* Id destructor method. */
static void	*Id_dtor(void *_self)
{
	struct s_Id	*self;

	self = super_dtor(Id, _self);
	return (self);
}

static char	*Id_str(const void *_self)
{
	const struct s_Id		*self = _self;
	const struct s_Word		*token = (struct s_Word *)get_op(self);

	return (str(token));
}

/* Return true if two Id are the same, false otherwise. */
static bool	Id_equal(const void *self, const void *other)
{
	return (super_equal(Id, self, other));
}

/* Get the value if an Id from the environment. */
static struct s_Token	*Id_eval(const void *self, void *env)
{
	struct s_Token	*retval = Env_get(env, (void *)self);
	char			*s;

	if (retval)
		return (copy(retval));
	s = str(get_op(self));
	fprintf(stderr, "'%s' is not defined.\n", s);
	free(s);
	return (NULL);
}

void	initId(void)
{
	initExpr();
	if (!Id)
		Id = new(ExprClass, "Id",
				Expr, sizeof(struct s_Id),
				ctor, Id_ctor,
				copy, Id_copy,
				dtor, Id_dtor,
				str, Id_str,
				equal, Id_equal,
				eval, Id_eval,
				0);
}
