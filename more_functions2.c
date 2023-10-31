#include "shell.h"

/**
 * _erratoi - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
char *process_plus_sign(char *s)
{
    char *p = s;

    do {
        p++;
    } while (*p == '+');
    return (p);
}

unsigned long int process_digits(char *s)
{
    unsigned long int result = 0;
    char current_char = *s;

    do {
        if (current_char >= '0' && current_char <= '9')
        {
            result = (result << 3) + (result << 1) + (current_char - '0');
            if (result > INT_MAX)
                return (-1);
        }
        else
            return (-1);

        current_char = *(++s);
    } while (current_char != '\0');

    return (result);
}

int _erratoi(char *s)
{
    s = process_plus_sign(s);

    return (process_digits(s));
}
/**
 * print_error - prints an error message
 * @info: the parameter & return info struct
 * @estr: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void print_message(char *message)
{
    _eputs(message);
    _eputs(": ");
}

void print_error_part1(info_t *info)
{
    print_message(info->fname);
}

void print_error_part2(info_t *info)
{
    print_d(info->line_count, STDERR_FILENO);
    _eputs(": ");
}

void print_error_part3(info_t *info, char *estr)
{
    print_message(info->argv[0]);
    print_message(estr);
}

void print_error(info_t *info, char *estr)
{
    print_error_part1(info);
    print_error_part2(info);
    print_error_part3(info, estr);
}

/**
 * print_d - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int handle_putchar(int fd)
{
    return ((fd == STDERR_FILENO) ? 1 : 0);
}

unsigned int handle_abs(int input, int (*__putchar)(char), int *count)
{
    unsigned int _abs_;

    if (input < 0)
    {
        _abs_ = -input;
        __putchar('-');
        (*count)++;
    }
    else
        _abs_ = input;
    return (_abs_);
}

int handle_current(unsigned int _abs_, int (*__putchar)(char), int *count)
{
    int i = 1000000000;
    unsigned int current = _abs_;
	
    do {
        if (_abs_ / i)
        {
            __putchar('0' + current / i);
            (*count)++;
        }
        current %= i;
        i /= 10;
    } while (i > 1);
    return (current);
}

void handle_last_digit(unsigned int current, int (*__putchar)(char), int *count)
{
    __putchar('0' + current);
    (*count)++;
}

int print_d(int input, int fd)
{
    int (*__putchar)(char) = _putchar;
    int count = 0;
    unsigned int _abs_, current;

    int is_stderr = handle_putchar(fd);
	
    __putchar = is_stderr ? _eputchar : _putchar;
    _abs_ = handle_abs(input, __putchar, &count);
    current = handle_current(_abs_, __putchar, &count);
    handle_last_digit(current, __putchar, &count);

    return (count);
}
/**
 * convert_number - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *handle_sign(long int num, int flags, unsigned long *n, char *sign)
{
	(!(flags & CONVERT_UNSIGNED) && num < 0) ? (*n = -num, *sign = '-') : (*n = num, *sign = 0);
	return ((flags & CONVERT_LOWERCASE) ? "0123456789abcdef" : "0123456789ABCDEF");
}

void convert_base(unsigned long *n, int base, char **ptr, char *array)
{
	do {
		*--(*ptr) = array[*n % base];
		*n /= base;
	} while (*n != 0);
}

void handle_negative_sign(char **ptr, char sign)
{
	if (sign)
		*--(*ptr) = sign;
}

char *convert_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n;

	array = handle_sign(num, flags, &n, &sign);
	ptr = &buffer[49];
	*ptr = '\0';

	do {

		convert_base(&n, base, &ptr, array);
	} while (n != 0);

	handle_negative_sign(&ptr, sign);

	return (ptr);
}

/**
 * remove_comments - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void set_to_null(char *p)
{
    do {
        *p = '\0';
        p++;
    } while(*p != '\0');
}

void remove_comments(char *buf)
{
    char *p = buf;

    do {
        if (*p == '#' && (p == buf || *(p - 1) == ' '))
            set_to_null(p);
        else
            p++;
    } while (*p != '\0');
}
