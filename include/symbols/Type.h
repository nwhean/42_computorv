#ifndef TYPE_H
# define TYPE_H

# include <stdbool.h>

# include "Word.h"

// all members are public
struct s_Type				// Type: Word
{
	const struct s_Word	_;
	int					width;
};

bool				type_numeric(struct s_Type *p);
const struct s_Type	*type_max(struct s_Type *p1, struct s_Type *p2);

extern const void	*Type;	// new(Type, TYPE, value);
extern const void	*Type_Int;
extern const void	*Type_Real;

void	initType(void);

#endif
