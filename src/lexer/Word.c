#include <string.h>

#include "Word.h"

const void	*Word;

/* Word constructor method. */
static void	*Word_ctor(void *_self, va_list *app)
{
	struct s_Word	*self;

	self = super_ctor(Word, _self, app);
	self->lexeme = strdup(va_arg(*app, char *));
	return (self);
}

/* Word destructor method. */
static void	*Word_dtor(void *_self, va_list *app)
{
	struct s_Word	*self = _self;

	free(self->lexeme);
	return (super_dtor(Word, _self));
}

/* Return string representing the Word. */
static const char	*Word_to_string(const void *_self)
{
	const struct s_Word	*self = _self;

	return strdup(self->lexeme);
}

void	initWord(void)
{
	if (!Word)
	{
		initToken();
		Word = new(TokenClass, "Word",
				Token, sizeof(struct s_Word),
				ctor, Word_ctor,
				dtor, Word_dtor,
				token_to_string, Word_to_string,
				0);
	}
}
