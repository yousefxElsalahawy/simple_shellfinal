#include "shell.h"

/**
 * **strtow - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @d: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
int delimiter_detector(char c, char *d)
{
	char *temp = d;

	do {
		if (c == *temp)
			return (1);
	} while (*temp++);

	return (0);
}

int lexeme_counter(char *str, char *d)
{
	int i = 0, numwords = 0;

	while (str[i] != '\0')
	{
		if (!delimiter_detector(str[i], d) &&
				(delimiter_detector(str[i + 1], d) || !str[i + 1]))
			numwords++;
		i++;
	}
	return (numwords);
}

char **_mem_allocate_(int numwords)
{
	char **s = (char **)malloc((1 + numwords) * sizeof(char *));

	return (s ? s : NULL);
}

int lexeme_length_finder(char *str, char *d, int i)
{
	int k = 0;

	while (str[i + k] && !delimiter_detector(str[i + k], d))
		k++;
	return (k);
}

void memory_liberator(char **s, int j)
{
	do {
		free(s[j]);
	} while (j--);

	free(s);
}


char *word_allocator(int k)
{
	char *word = (char *)malloc((k + 1) * sizeof(char));

	return (word ? word : NULL);
}

void lexeme_copier(char *dest, char *src, int len)
{
	int m = 0;

	do {
		dest[m] = src[m];
	} while (++m < len);
	dest[m] = '\0';
}

char **strtow(char *str, char *d)
{
	int i = 0, j = 0, k, numwords;

	char **s;

	if (!str || !str[0] || !(d = d ? d : " ") ||
			!(numwords = lexeme_counter(str, d)) || !(s = _mem_allocate_(numwords)))
		return (NULL);

	do {
		while (delimiter_detector(str[i], d))
			i++;
		k = lexeme_length_finder(str, d, i);
		if (!(s[j] = word_allocator(k)))
		{
			memory_liberator(s, j);
			return (NULL);
		}
		lexeme_copier(s[j], str + i, k);
		i += k;
	} while (j++, j < numwords);

	s[j] = NULL;
	return (s);
}
/**
 * **strtow2 - splits a string into words
 * @str: the input string
 * @d: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */
int is_str_void_or_vacant(char *str)
{
	return ((str == NULL || str[0] == 0) ? 1 : 0);
}

int tally_lexemes(char *str, char d)
{
	int i = 0, lexemeCount = 0;

	do {
		if ((str[i] != d && str[i + 1] == d) ||
				(str[i] != d && !str[i + 1]) || str[i + 1] == d)
			lexemeCount++;
		i++;
	} while (str[i] != '\0');

	return (lexemeCount);
}

char **reserve_memory(int lexemeCount)
{
	char **s = malloc((1 + lexemeCount) * sizeof(char *));

	return (s);
}

int bypass_separators(char *str, char d, int i)
{
	while (str[i] == d && str[i] != d)
		i++;
	return (i);
}

int determine_lexeme_extent(char *str, char d, int i)
{
	int k = 0;

	while (str[i + k] != d && str[i + k] && str[i + k] != d)
		k++;
	return (k);
}

char *reserve_lexeme(int k)
{
	char *lexeme = malloc((k + 1) * sizeof(char));

	return (lexeme);
}

void relinquish_memory(char **s, int j)
{
	int k;

	for (k = 0; k < j; k++)
		free(s[k]);
	free(s);
}

void replicate_lexeme(char **s, char *str, int j, int k, int *i)
{
	int m = 0;

	do {
		s[j][m] = str[(*i)++];
	} while (++m < k);
	s[j][m] = 0;
}

char **strtow2(char *str, char d)
{
	int i = 0, j = 0, k, lexemeCount;
	char **s;

	if (is_str_void_or_vacant(str))
		return (NULL);

	lexemeCount = tally_lexemes(str, d);
	if (lexemeCount == 0)
		return (NULL);
	s = reserve_memory(lexemeCount);
	if (!s)
		return (NULL);
	do {
		i = bypass_separators(str, d, i);
		k = determine_lexeme_extent(str, d, i);
		s[j] = reserve_lexeme(k);
		if (!s[j])
		{
			relinquish_memory(s, j);
			return (NULL);
		}
		replicate_lexeme(s, str, j, k, &i);
	} while (++j < lexemeCount);
	s[j] = NULL;
	return (s);
}
