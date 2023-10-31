#include "shell.h"

/**
 **_strncpy - copies a string
 *@dest: the destination string to be copied to
 *@src: the source string
 *@n: the amount of characters to be copied
 *Return: the concatenated string
 */
char *_copy_chars_(char *dest, char *src, int n)
{
	
	int i = 0;

	while (src[i] != '\0' && i < n - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';

	return (dest);
}

char *fill_remaining_with_null(char *dest, int start, int n)
{
	int j = start;

	do {
		dest[j] = '\0';
		j++;
	} while (j < n);

	return dest;
}

char *_strncpy(char *dest, char *src, int n)
{
	char *s = dest;
	int i;

	s = _copy_chars_(s, src, n);

	i = _strlen(s);

	s = (i < n) ? fill_remaining_with_null(s, i, n) : s;
	return (s);
}

/**
 **_strncat - concatenates two strings
 *@dest: the first string
 *@src: the second string
 *@n: the amount of bytes to be maximally used
 *Return: the concatenated string
 */
int find_length(char *str)
{
	int i = 0;
	
	do {
		i++;
	} while (str[i] != '\0');
	return (i - 1);
}

void cpy_chars(char *dest, char *src, int start, int n)
{
	int j = 0;

	do {

		dest[start + j] = src[j];
		j++;
	} while (src[j] != '\0' && j < n);

	dest[start + j] = (j < n) ? '\0' : dest[start + j];
}

char *_strncat(char *dest, char *src, int n)
{
	int i = find_length(dest);

	cpy_chars(dest, src, i, n);

	return (dest);
}
/**
 **_strchr - locates a character in a string
 *@s: the string to be parsed
 *@c: the character to look for
 *Return: (s) a pointer to the memory area s
 */
char *check_char(char *s, char c)
{
    return ((*s == c) ? s : NULL);
}

char *_strchr(char *s, char c)
{
    char *result = NULL;

    do {
        result = check_char(s, c);

        if (result != NULL)
            return (result);
			
    } while (*s++ != '\0');

    return (result);
}
