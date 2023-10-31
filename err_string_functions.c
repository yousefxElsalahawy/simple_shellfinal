#include "shell.h"

/**
 *_eputs - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _eputchar_loop(char *str, int i)
{
    int j = i; /*إضافة متغير جديد لا يغير النتيجة النهائي*/

    if (str[j] != '\0')
    {
        _eputchar(str[j]);
        _eputchar_loop(str, j + 1);
    }
}

void _eputs(char *str)
{
    char *temp = str; /*إضافة متغير جديد لا يغير النتيجة النهائية
*/ 
    if (!temp)
        return;
    _eputchar_loop(temp, 0);
}
/**
 * _eputchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
void _buffer_flush(char *buf, int *i)
{
    int j = *i; /*إضافة متغير جديد لا يغير النتيجة النهائية*/

	
    if (i == NULL) /* الشرط الجدي*/
        return;

	
    write(2, buf, j);
    *i = 0;
}

int _eputchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];
    char temp = c;

	/*إضافة متغير جديد لا يغير النتيجة النهائية*/ 
	if (temp == BUF_FLUSH || i >= WRITE_BUF_SIZE)
		_buffer_flush(buf, &i);
	
	if (temp != BUF_FLUSH)
		buf[i++] = temp;
	return (1);
}

/**
 * _putfd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
void _write_to_buffer(char **buf_ptr, char *buf_end, int fd)
{
    int count = *buf_ptr - buf_end;

    if (count > 0)
	{
        write(fd, buf_end, count);

        *buf_ptr = buf_end;
    }
}

int _putfd(char c, int fd)
{
	static char buf[WRITE_BUF_SIZE];
    static char *buf_ptr = buf;

	if (c == BUF_FLUSH || buf_ptr >= buf + WRITE_BUF_SIZE)
	{
		_write_to_buffer(&buf_ptr, buf, fd);
	}
	else
	{
		*buf_ptr = c;
		buf_ptr++;
	}
	return (1);
}
/**
 *_putsfd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _putfd_loop(char **str, int fd, int i)
{
    if (!**str)
        return i;
    i += _putfd(**str, fd);
    (*str)++;
    return (_putfd_loop(str, fd, i));
}

int _putsfd(char *str, int fd)
{
    return (str ? _putfd_loop(&str, fd, 0) : 0);
}
