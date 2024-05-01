#include <string.h>

#include "Str.h"
#include "UnorderedMap.h"
#include "Word.h"

const struct s_Word	*Word;
const struct s_Word	*Word_plus;
const struct s_Word	*Word_minus;
const struct s_Word	*Word_mmult;
const struct s_Word	*Word_imag;
const void			*Word_reserved;

/* forward declaration */
static void	reserve_add(const struct s_Word *word);
static const struct s_Word *reserve_find(const void *str);

/* Word constructor method.
 * If a Word is reserved, the original is returned instead, i.e. Singleton.
 */
static void	*Word_ctor(void *_self, va_list *app)
{
	struct s_Word		*self;
	const struct s_Word	*retval;

	self = super_ctor(Word, _self, app);
	self->lexeme = new(Str, va_arg(*app, char *));
	retval = reserve_find(self->lexeme);
	if (!retval)
		return (self);

	/* free the resources, if a reserved word is identified. */
	delete(self->lexeme);
	super_dtor(Word, _self);
	return ((struct s_Word *)retval);
}

/* Word destructor method. */
static void	*Word_dtor(void *_self)
{
	struct s_Word		*self = _self;
	const struct s_Word	*find;

	/* avoid destructing some reserved Words */
	find = reserve_find(self->lexeme);
	if (find)
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
	struct s_Word	*retval;

	retval = (struct s_Word *)reserve_find(self->lexeme);
	if (!retval)
	{
		retval = super_copy(Word, self);
		retval->lexeme = copy(self->lexeme);
		return (retval);
	}
	return (retval);
}

/* Return string representing the Word. */
static char	*Word_str(const void *_self)
{
	const struct s_Word	*self = _self;

	return (str(self->lexeme));
}

/* add a word into the reserve. */
static void	reserve_add(const struct s_Word *word)
{
	UnorderedMap_insert(
		(void *)Word_reserved, copy(word->lexeme), (void *)word);
}

/* Find a Word regestered in the the reserve. */
static const struct s_Word *reserve_find(const void *str)
{
	return (UnorderedMap_find((void *)Word_reserved, str));
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
				copy, Word_copy,
				dtor, Word_dtor,
				str, Word_str,
				0);
		Word_reserved = new(UnorderedMap);
		Word_plus = new(Word, PLUS, "plus");
		Word_minus = new(Word, MINUS, "minus");
		Word_mmult = new(Word, MMULT, "mmult");
		Word_imag = new(Word, IMAG, "i");

		/* Insert data into reserve. */
		reserve_add(Word_plus);
		reserve_add(Word_minus);
		reserve_add(Word_mmult);
		reserve_add(Word_imag);
	}
}
