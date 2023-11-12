#include "shell.h"

/**
 * _eputchar_loop - This function prints a character to the standard error
 * @str: This pointer refers to the string to be printed
 * @i: This variable is used as the index for the string
 */
void _eputchar_loop(char *str, int i)
{
	int j = i; /*declaration*/

	if (str[j] != '\0') /*use if*/
	{
		_eputchar(str[j]);
		_eputchar_loop(str, j + 1);
	}
}

/**
 * _eputs - This function prints a string to the standard error
 * @str: This pointer refers to the string to be printed
 */
void _eputs(char *str)
{
	char *temp = str; /*declaration*/

	if (!temp)
		return;
	_eputchar_loop(temp, 0);
}

/**
 * _buffer_flush - This function flushes the buffer
 * @buf: This pointer refers to the buffer to be flushed
 * @i: This variable is used as the index for the buffer
 */
void _buffer_flush(char *buf, int *i)
{
	int j = *i; /*declaration*/

	if (i == NULL) /*use if*/
		return;

	write(2, buf, j);
	*i = 0;
}

/**
 * _eputchar - This function writes a character to the buffer
 * @c: This variable is the character to be written
 *
 * Return: 1 (success)
 */
int _eputchar(char c)
{
	static int i; /*declaration*/
	static char buf[WRITE_BUF_SIZE]; /*declaration*/
	char temp = c; /*declaration*/

	if (temp == BUF_FLUSH || i >= WRITE_BUF_SIZE) /*use if*/
		_buffer_flush(buf, &i);

	if (temp != BUF_FLUSH) /*use if*/
		buf[i++] = temp;
	return (1); /*returns 1 on success*/
}

/**
 * _write_to_buffer - This function writes to a buffer
 * @buf_ptr: This pointer refers to the buffer pointer
 * @buf_end: This pointer refers to the end of the buffer
 * @fd: This variable is the file descriptor
 */
void _write_to_buffer(char **buf_ptr, char *buf_end, int fd)
{
	int count = *buf_ptr - buf_end; /*declaration*/

	if (count > 0) /*use if*/
	{
		write(fd, buf_end, count);

		*buf_ptr = buf_end;
	}
}

/**
 * _putfd - This function writes a character to a file descriptor
 * @c: This variable is the character to be written
 * @fd: This variable is the file descriptor
 *
 * Return: 1 (success)
 */
int _putfd(char c, int fd)
{
	static char buf[WRITE_BUF_SIZE]; /*declaration*/
	static char *buf_ptr = buf; /*declaration*/

	if (c == BUF_FLUSH || buf_ptr >= buf + WRITE_BUF_SIZE) /*use if*/
	{
		_write_to_buffer(&buf_ptr, buf, fd);
	}
	else
	{
		*buf_ptr = c;
		buf_ptr++;
	}
	return (1); /*returns 1 on success*/
}

/**
 * _putfd_loop - This function writes a string to a file descriptor
 * @str: This pointer refers to the string to be written
 * @fd: This variable is the file descriptor
 * @i: This variable is used as the index for the string
 *
 * Return: i (the number of characters written)
 */
int _putfd_loop(char **str, int fd, int i)
{
	if (!**str) /*use if*/
		return (i);

	i += _putfd(**str, fd);
	(*str)++;
	return (_putfd_loop(str, fd, i));
	/*returns the number of characters written*/
}

/**
 * _putsfd - This function writes a string to a file descriptor
 * @str: This pointer refers to the string to be written
 * @fd: This variable is the file descriptor
 *
 * Return: the number of characters written, or 0 if the string is NULL
 */
int _putsfd(char *str, int fd)
{
	return (str ? _putfd_loop(&str, fd, 0) : 0);

	/*returns the number of characters written, or 0 if the string is NULL*/
}
