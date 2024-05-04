#ifndef ENV_R
# define ENV_R

# include "Object.r"

/* container */
# include "UnorderedMap.h"

/* Base class from which Expr and Stmt classes derives from. */
struct s_Env	/* Env: Object */
{
	const struct s_Object	_;
	void					*table;	/* UnorderedMap */
	struct s_Env			*prev;
};

struct	s_EnvClass	/* EnvClass: Class */
{
	const struct s_Class	_;
};

#endif
