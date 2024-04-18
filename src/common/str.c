#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Return a pointer to a new string which is a duplicate of the string `s`.*/
char	*strdup(const char *s)
{
	size_t	len = strlen(s);
	char	*retval = malloc(len + 1);

	memcpy(retval, s, len + 1);
	return (retval);
}
