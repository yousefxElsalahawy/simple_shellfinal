#include "shell.h"

/**
 * interactive - returns true if shell is interactive mode
 * @info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive(info_t *info)
{
	int result;
	int is_interactive = isatty(STDIN_FILENO);

	int is_readfd_valid = info->readfd <= 2;

	result = is_interactive && is_readfd_valid;

	return (result);
}
/**
 * is_delim - checks if character is a delimeter
 * @c: the char to check
 * @delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int _CHEcK_chARs_(char c, char *temp)
{
	while (*temp)
	{
		if (c == *temp++)
		{
			return (1);
		}
	}
	return (0);
}

int is_delim(char c, char *delim)
{
	return (_CHEcK_chARs_(c, delim));
}

/**
 *_isalpha - checks for alphabetic character
 *@c: The character to input
 *Return: 1 if c is alphabetic, 0 otherwise
 */

int _islower(int c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	else
		return (0);
}

int _isupper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	else
		return (0);
}

int _isalpha(int c)
{
	if (_islower(c) || _isupper(c))
		return (1);
	else
		return (0);
}
/**
 *_atoi - converts a string to an integer
 *@s: the string to be converted
 *Return: 0 if no numbers in string, converted number otherwise
 */

int process_sign(char s, int sign)
{
	return ((s == '-') ? sign * -1 : sign);
}

unsigned int process_digit(char s, unsigned int result, int *flag)
{
	if (s >= '0' && s <= '9')
	{
		*flag = 1;
		result = result * 10 + (s - '0');
	}
	else if (*flag == 1)
		*flag = 2;
	return (result);
}

int _atoi(char *s)
{
	int i = 0, sign = 1, flag = 0;
	unsigned int result = 0;

	do {
		sign = process_sign(s[i], sign);
		result = process_digit(s[i], result, &flag);
		i++;
	} while (s[i] != '\0' && flag != 2);

	return ((sign == -1) ? -result : result);
}
