#include <string.h>

#include "Word.h"

const struct s_Word		*Word;
const struct s_Word		*Word_minus;

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
	struct s_Word		*self = _self;
	const struct s_Word	*reserved[] = {Word_minus};
	size_t				len = sizeof(reserved) / sizeof(struct s_Word *);

	// avoid destructing some reserved Words
	for (size_t i = 0; i < len; ++i)
	{
		if (self == reserved[i])
			return (NULL);
	}
	free(self->lexeme);
	return (super_dtor(Word, _self));
}

/* Return a copy of the Word. */
static struct s_Word	*Word_copy(const void *_self)
{
	const struct s_Word	*self = _self;

	return new(Word, get_tag(self), self->lexeme);
}

/* Return string representing the Word. */
static const char	*Word_to_string(const void *_self)
{
	const struct s_Word	*self = _self;

	return (strdup(self->lexeme));
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
				token_copy, Word_copy,
				token_to_string, Word_to_string,
				0);
		Word_minus = new(Word, MINUS, "minus");
	}
}
