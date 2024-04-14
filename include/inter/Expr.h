#ifndef EXPR_H
# define EXPR_H

# include "Node.h"
# include "Node.r"
# include "Token.h"

struct s_Expr			// Expr: Node
{
	const struct s_Node	_;
	struct s_Token		*op;	// operator of the node
	struct s_Type		*type;	// type of the node
};

extern const void	*Expr;			// new(Expr, Token, Type);

struct	s_ExprClass	// ExprClass: NodeClass
{
	const struct s_NodeClass	_;
	struct s_Expr				*(*gen)(const void *self);
	struct s_Expr				*(*reduce)(const void *self);
	const char					*(*to_string)(const void *self);
	const struct s_Token		*(*eval)(const void *self);
};

extern const void		*ExprClass;
struct s_Expr			*gen(const void *self);
struct s_Expr			*reduce(const void *self);
const char				*to_string(const void *self);
const struct s_Token	*eval(const void *self);
const struct s_Token	*get_op(const void *self);
void					set_type(void *self, const struct s_Type *type);
const struct s_Type		*get_type(const void *self);

void		initExpr(void);

#endif
