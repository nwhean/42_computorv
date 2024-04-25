#ifndef NUMERIC_H
# define NUMERIC_H

# include <stdbool.h>

# include "Token.h"

/* all members are public */
struct s_Numeric	/* Numeric: Token */
{
	const struct s_Token	_;
};

extern const void	*Numeric;	/* new(Numeric, NULL); */

struct	s_NumericClass	/* NumericClass: TokenClass */
{
	const struct s_TokenClass	_;
	struct s_Numeric			*(*add)(const void *self, const void *other);
	struct s_Numeric			*(*sub)(const void *self, const void *other);
	struct s_Numeric			*(*mul)(const void *self, const void *other);
	struct s_Numeric			*(*div)(const void *self, const void *other);
	struct s_Numeric			*(*mod)(const void *self, const void *other);
	struct s_Numeric			*(*pos)(const void *self);
	struct s_Numeric			*(*neg)(const void *self);
	struct s_Numeric			*(*pow)(const void *self, const void *other);
	struct s_Numeric			*(*promote)(const void *self, enum e_Tag tag);
};

extern const void	*NumericClass;
void				*numeric_add(const void *self, const void *other);
void				*numeric_sub(const void *self, const void *other);
void				*numeric_mul(const void *self, const void *other);
void				*numeric_div(const void *self, const void *other);
void				*numeric_mod(const void *self, const void *other);
void				*numeric_pos(const void *self);
void				*numeric_neg(const void *self);
void				*numeric_pow(const void *self, const void *other);
bool				numeric_equal(const void *self, const void *other);
void				*numeric_promote(const void *self, enum e_Tag tag);

void				initNumeric(void);
bool				numeric_is(enum e_Tag tag);
enum e_Tag			numeric_max(enum e_Tag tag1, enum e_Tag tag2);

#endif
