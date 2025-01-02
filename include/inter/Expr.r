#ifndef EXPR_R
# define EXPR_R

struct s_Token	*super_eval(const void *_class, const void *_self, void *env);
struct s_Token	*super_to_polynomial(
					const void *_class, const void *_self, void *env);

#endif
