/* container */
#include "Str.h"

/* inter */
#include "Seq.h"
#include "Stmt.h"

const void	*Seq;

/* Seq constructor method. */
static void	*Seq_ctor(void *_self, va_list *app)
{
	struct s_Seq	*self;

	self = super_ctor(Seq, _self, app);
	self->stmt1 = va_arg(*app, struct s_Stmt *);
	self->stmt2 = va_arg(*app, struct s_Stmt *);
	return (self);
}

/* Seq destructor method. */
static void	*Seq_dtor(void *_self)
{
	struct s_Seq	*self = _self;

	delete(self->stmt1);
	delete(self->stmt2);
	return (super_dtor(Seq, _self));
}

static char	*Seq_str(const void *_self)
{
	const struct s_Seq	*self = _self;

	void	*s = new(Str, "");
	char	*str_append;
	char	*retval;

	if (self->stmt1)
	{
		str_append = str(self->stmt1);
		Str_append(s, str_append);
		free(str_append);
	}
	if (self->stmt2)
	{
		str_append = str(self->stmt2);
		Str_append(s, str_append);
		free(str_append);
	}
	retval = str(s);
	delete(s);
	return (retval);
}

static struct s_Token	*Seq_eval(const void *_self)
{
	const struct s_Seq	*self = _self;

	if (self->stmt1)
		eval(self->stmt1);
	if (self->stmt2)
		eval(self->stmt2);
	return (NULL);
}

void	initSeq(void)
{
	initStr();
	initStmt();
	if (!Seq)
		Seq = new(NodeClass, "Seq",
				Node, sizeof(struct s_Seq),
				ctor, Seq_ctor,
				dtor, Seq_dtor,
				str, Seq_str,
				eval, Seq_eval,
				0);
}
