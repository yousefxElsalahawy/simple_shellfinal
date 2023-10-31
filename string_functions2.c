#include "shell.h"

/**
 * _strcpy - copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *_copy(char *dest, char *src, int i)
{
    if (src[i])
    {
        dest[i] = src[i];
        return _copy(dest, src, i + 1);
    }
    dest[i] = 0;
    return (dest);
}

char *_strcpy(char *dest, char *src)
{
        return ((dest == src || src == 0) ? dest : _copy(dest, src, 0));

}

/**
 * _strdup - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
int _strlen___(const char *str)
{
    int length = 0;
    const char *s = str ? str : "";

    while (*s)
    {
        length++;
        s++;
    }
    return (length);
}

char *_strdup(const char *str)
{
    int length = _strlen___(str);
    char *ret = (char *) malloc(sizeof(char) * (length + 1));

    if (ret)
    {
        char *p = ret;
        const char *s = str ? str : "";

        while (*s)
        {
            *p = *s;
            p++;
            s++;
        }
        *p = '\0';
    }
    return (ret);
}

/**
 *_puts - prints an input string
 *@str: the string to be printed
 *
 * Return: Nothing
 */
void _print_char(char *str, char *end)
{
    if (str != end)
    {
        _putchar(*str);
        _print_char(str + 1, end);
    }
}

void _puts(char *str)
{
    char *end = str;

    while (*end)
		++end;
	if (str)
	{
		_print_char(str, end);
	}
	else
	{
		return;
	}
}








/*void _print_char(char *str, int *i)
{
    do {
        _putchar(str[*i]);
        (*i)++;
    } while (str[*i] != '\0');
}

void _puts(char *str)
{
    int i = 0;
    str ? _print_char(str, &i) : return;
}*/



/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
void write_buffer(char *buf, int *i)
{
    write(1, buf, *i);
    *i = 0;
}

void add_to_buffer(char *buf, int *i, char c)
{
    buf[(*i)++] = c;
}

int _putchar(char c)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write_buffer(buf, &i);
    }
    else
    {
        add_to_buffer(buf, &i, c);
    }
    return (1);
}
