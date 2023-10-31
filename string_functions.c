#include "shell.h"

/**
 * _strlen - returns the length of a string
 * @s: the string whose length to check
 *
 * Return: integer length of string
 */
int _increment_while_not_null(char **s, int *i)
{
    while (*(*s)++)
        (*i)++;
		
    return (*i);
}

int _strlen(char *s)
{
    int i = 0;

    return ((!s) ? 0 : _increment_while_not_null(&s, &i));
}

/**
 * _strcmp - performs lexicogarphic comparison of two strangs.
 * @s1: the first strang
 * @s2: the second strang
 *
 * Return: negative if s1 < s2, positive if s1 > s2, zero if s1 == s2
 */
int compare_chars(char c1, char c2)
{
	if (c1 != c2)
			return (c1 - c2);
		else if (c1 == c2)
			return (0);
		else
			return (c1 < c2 ? -1 : 1);
}

int _strcmp(char *s1, char *s2)
{
    int result = 0;

    do {
        result = compare_chars(*s1, *s2);
        if (result != 0)
            return (result);
        s1++;
        s2++;
    } while (*s1 && *s2);
    return (compare_chars(*s1, *s2));
}
/**
 * starts_with - checks if needle starts with haystack
 * @haystack: string to search
 * @needle: the substring to find
 *
 * Return: address of next char of haystack or NULL
 */
int check_chars(const char **needle, const char **haystack)
{
    return ((*(*needle)++ == *(*haystack)++) ? 1 : 0);
}

char *starts_with(const char *haystack, const char *needle)
{
    const char *temp_needle = needle;
    const char *temp_haystack = haystack;

    do {
        if (!check_chars(&temp_needle, &temp_haystack))
            return (NULL);
    } while (*temp_needle);
	
    return ((char *)temp_haystack);
}

/**
 * _strcat - concatenates two strings
 * @dest: the destination buffer
 * @src: the source buffer
 *
 * Return: pointer to destination buffer
 */
char *find_end(char *str)
{
    char *end = str;

    for (; *end; ++end);
    return (end);
}

void copy_str(char *dest, char *src)
{
    for (; *src; ++src, ++dest)
        *dest = *src;
    *dest = '\0';
}

char *_strcat(char *dest, char *src)
{
    char *ret = dest;
    char *end_of_dest = find_end(dest);

    copy_str(end_of_dest, src);
    return (ret);
}
