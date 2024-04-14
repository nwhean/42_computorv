#ifndef TOKEN_R
# define TOKEN_R

#include "Token.h"

void		*super_token_copy(const void *_class, const void *_self);
const char	*super_token_to_string(const void *_class, const void *_self);
enum e_Tag	token_get_tag(const void *self);

#endif
