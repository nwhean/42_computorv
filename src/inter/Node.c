#include <assert.h>

#include "Node.r"
#include "Node.h"

const void	*Node;
const void	*NodeClass;

/* Node constructor method. */
static void	*Node_ctor(void *_self, va_list *app)
{
	struct s_Node	*self;

	self = super_ctor(Node, _self, app);
	return (self);
}

/* Return Token representing the Expr. */
struct s_Token	*eval(const void *self)
{
	const struct s_NodeClass *const	*cp = self;

	assert(self && *cp && (*cp)->eval);
	return ((*cp)->eval(self));
}

struct s_Token	*super_eval(const void *_class, const void *_self)
{
	const struct s_NodeClass	*superclass = super(_class);

	assert(_self && superclass->eval);
	return (superclass->eval(_self));
}

/* NodeClass constructor method. */
static void	*NodeClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_NodeClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(NodeClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		method = va_arg(ap, voidf);
		if (selector == (voidf)eval)
			*(voidf *)&self->eval = method;
		#pragma GCC diagnostic pop
	}
	return (self);
}

void	initNode(void)
{
	if (!NodeClass)
		NodeClass = new(Class, "NodeClass",
				Class, sizeof(struct s_NodeClass),
				ctor, NodeClass_ctor,
				0);
	if (!Node)
		Node = new(NodeClass, "Node",
				Object, sizeof(struct s_Node),
				ctor, Node_ctor,
				0);
}
