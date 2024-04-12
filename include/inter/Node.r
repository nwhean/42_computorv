#ifndef NODE_R
# define NODE_R

# include "Object.r"

/* Base class from which Expr and Stmt classes derives from. */
struct s_Node			// Node: Object
{
	const struct s_Object	_;
};

struct	s_NodeClass	// NodeClass: Class
{
	const struct s_Class	_;
};

#endif
