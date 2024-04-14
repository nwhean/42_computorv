#ifndef EXPR_R
# define EXPR_R

struct s_Expr			*super_gen(const void *_class, const void *_self);
struct s_Expr			*super_reduce(const void *_class, const void *_self);
const char				*super_to_string(const void *_class, const void *_self);
const struct s_Token	*super_eval(const void *_class, const void *_self);

#endif
