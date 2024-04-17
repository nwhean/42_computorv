#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Return true if two string `s1` and `s2` are equal. */
bool	str_equal(const char *s1, const char *s2)
{
	return (strcmp(s1, s2) == 0);
}

/* Return a pointer to a new string which is a duplicate of the string `s`.*/
char	*strdup(const char *s)
{
	size_t	len = strlen(s);
	char	*retval = malloc(len + 1);

	memcpy(retval, s, len + 1);
	return (retval);
}
