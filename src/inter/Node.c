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

/* NodeClass constructor method. */
static void	*NodeClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_NodeClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(NodeClass, _self, app);
	va_copy(ap, *app);
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		method = va_arg(ap, voidf);
		(void) method;
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