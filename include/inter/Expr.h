#ifndef EXPR_H
# define EXPR_H

# include "Node.h"
# include "Node.r"
# include "Token.h"

struct s_Expr	/* Expr: Node */
{
	const struct s_Node	_;
	struct s_Token	*op;	/* operator of the node */
	enum e_Tag		tag;	/* type of the node */
};

extern const void	*Expr;	/* new(Expr, Token, Tag); */

struct	s_ExprClass	/* ExprClass: NodeClass */
{
	const struct s_NodeClass	_;
	const struct s_Token		*(*eval)(const void *self);
};

extern const void		*ExprClass;
const struct s_Token	*eval(const void *self);
const struct s_Token	*get_op(const void *self);
void					set_tag(void *self, enum e_Tag tag);
enum e_Tag				get_tag(const void *self);

void		initExpr(void);

#endif
