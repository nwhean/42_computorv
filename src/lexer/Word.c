#include <string.h>

#include "Str.h"
#include "UnorderedMap.h"
#include "Word.h"

const struct s_Word	*Word;
const struct s_Word	*Word_plus;
const struct s_Word	*Word_minus;
const void			*Word_reserved;

/* forward declaration */
static void	reserve_add(const char *s, const struct s_Word *word);
static struct s_Word *reserve_find(const char *s);

/* Word constructor method.
 * If a Word is reserved, the original is returned instead, i.e. Singleton.
 */
static void	*Word_ctor(void *_self, va_list *app)
{
	struct s_Word	*self;
	struct s_Word	*retval;

	self = super_ctor(Word, _self, app);
	self->lexeme = new(Str, va_arg(*app, char *));
	retval = reserve_find(self->lexeme);
	if (!retval)
		return (self);

	/* free the resources, if a reserved word is identified. */
	delete(self->lexeme);
	super_dtor(Word, _self);
	return (retval);
}

/* Word destructor method. */
static void	*Word_dtor(void *_self)
{
	struct s_Word	*self = _self;
	struct s_Word	*retval;

	/* avoid destructing some reserved Words */
	retval = reserve_find(self->lexeme);
	if (retval)
		return (NULL);
	delete(self->lexeme);
	return (super_dtor(Word, _self));
}

/* Return a copy of the Word.
 * If a Word is a reserved, the original is retured instead (i.e. Singleton).
 */
static struct s_Word	*Word_copy(const void *_self)
{
	const struct s_Word	*self = _self;
	struct s_Word		*retval;

	retval = reserve_find(self->lexeme);
	if (!retval)
		return new(Word, token_get_tag(self), Str_c_str(self->lexeme));
	return (retval);
}

/* Return string representing the Word. */
static const char	*Word_to_string(const void *_self)
{
	const struct s_Word	*self = _self;

	return (strdup(Str_c_str(self->lexeme)));
}

/* add a word into the reserve. */
static void	reserve_add(const char *s, const struct s_Word *word)
{
	UnorderedMap_insert((void *)Word_reserved, new(Str, s), (void *)word);
}

/* Find a Word regestered in the the reserve. */
static struct s_Word *reserve_find(const char *s)
{
	void			*str = new(Str, s);
	struct s_Word	*retval = UnorderedMap_find((void *)Word_reserved, str);

	delete(str);
	return (retval);
}

void	initWord(void)
{
	initToken();
	initUnorderedMap();
	if (!Word)
	{
		Word = new(TokenClass, "Word",
				Token, sizeof(struct s_Word),
				ctor, Word_ctor,
				dtor, Word_dtor,
				token_copy, Word_copy,
				token_to_string, Word_to_string,
				0);
		Word_reserved = new(UnorderedMap, Str_compare);
		Word_plus = new(Word, PLUS, "plus");
		Word_minus = new(Word, MINUS, "minus");

		/* Insert data into reserve. */
		reserve_add("plus", Word_plus);
		reserve_add("minus", Word_minus);
	}
}
