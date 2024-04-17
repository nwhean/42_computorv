#include "str.h"
#include "Container.h"
#include "UnorderedMap.h"
#include "Word.h"

const struct s_Word	*Word;
const struct s_Word	*Word_plus;
const struct s_Word	*Word_minus;
const void			*reserved;

/* Word constructor method. */
static void	*Word_ctor(void *_self, va_list *app)
{
	struct s_Word	*self;

	self = super_ctor(Word, _self, app);
	self->lexeme = strdup(va_arg(*app, char *));
	return (self);
}

/* Word destructor method. */
static void	*Word_dtor(void *_self)
{
	struct s_Word		*self = _self;

	/* avoid destructing some reserved Words */
	if (!container_find(reserved, self->lexeme))
		free(self->lexeme);
	return (super_dtor(Word, _self));
}

/* Return a copy of the Word. */
static struct s_Word	*Word_copy(const void *_self)
{
	const struct s_Word	*self = _self;

	return new(Word, token_get_tag(self), self->lexeme);
}

/* Return string representing the Word. */
static const char	*Word_to_string(const void *_self)
{
	const struct s_Word	*self = _self;

	return (strdup(self->lexeme));
}

/* add a word into the reserve. */
static void	reserve_add(const char *s, const struct s_Word *word)
{
	struct s_Pair	pair;

	pair.first = (void *)s;
	pair.second = (void *)word;
	container_insert((void *)reserved, &pair);
}

void	initWord(void)
{
	if (!Word)
	{
		initToken();
		initUnorderedMap();
		Word = new(TokenClass, "Word",
				Token, sizeof(struct s_Word),
				ctor, Word_ctor,
				dtor, Word_dtor,
				token_copy, Word_copy,
				token_to_string, Word_to_string,
				0);
		Word_plus = new(Word, PLUS, "plus");
		Word_minus = new(Word, MINUS, "minus");
		reserved = new(UnorderedMap, str_equal);

		/* Insert data into reserve. */
		reserve_add("plus", Word_plus);
		reserve_add("minus", Word_minus);
	}
}
