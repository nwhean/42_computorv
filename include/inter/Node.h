#ifndef NODE_H
# define NODE_H

# include "Object.h"

extern const void	*Node;	/* new(Node); */

extern const void	*NodeClass;

struct s_Token	*eval(const void *self);

void			initNode(void);

#endif
