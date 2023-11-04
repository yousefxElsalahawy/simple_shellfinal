#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */

void reset_buffer(__attribute__((unused)) info_t *info, char **buf)
{
	if (*buf)
	{
		free(*buf);
		*buf = NULL;
	}
	signal(SIGINT, sigintHandler);
}

ssize_t input_reader(info_t *info, char **buf, size_t *len_p)
{
	ssize_t r = 0;

#if USE_GETLINE
	r = getline(buf, len_p, stdin);
#else
	r = _getline(info, buf, len_p);
#endif

	return (r);
}

void input_processor(info_t *info, char **buf, ssize_t *r)
{
	(*buf)[*r - 1] == '\n' ? (*buf)[--(*r)] = '\0' : 0;

	info->linecount_flag = 1;
	remove_comments(*buf);
	build_history_list(info, *buf, info->histcount++);
}

void command_chain_handler(info_t *info, char **buf, size_t *len, ssize_t r)
{
	*len = r;
	info->cmd_buf = buf;
}
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len)
	{
		reset_buffer(info, buf);
		r = input_reader(info, buf, &len_p);
		if (r > 0)
		{
			input_processor(info, buf, &r);
			command_chain_handler(info, buf, len, r);
		}
	}
	return (r);
}





/**
 * get_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t handle_input(info_t *info, char **buf, size_t *len)
{
	ssize_t r;

    _putchar(BUF_FLUSH);

    r = input_buf(info, buf, len);

    return ((r == -1) ? -1 : r);
}

void init_iterator(size_t *i, size_t *j, char *buf, char **p)
{
    *j = *i; /* init new iterator to current buf position */
    *p = buf + *i; /* get pointer for return */
}

void iterate_to_semicolon_or_end(info_t *info, char *buf, size_t *j, size_t len)
{
    do {
        if (is_chain(info, buf, j))
            break;
        (*j)++;
    } while (*j < len); /* iterate to semicolon or end */
}

void handle_chain(info_t *info, char *buf, size_t *i, size_t *j, size_t len, char **p)
{
    init_iterator(i, j, buf, p);
    check_chain(info, buf, j, *i, len);
    iterate_to_semicolon_or_end(info, buf, j, len);
}

void _RESet_BuFFer_(info_t *info, size_t *i, size_t *len)
{
    *i = *len = 0; /* reset position and length */
    info->cmd_buf_type = CMD_NORM;
}

ssize_t get_input(info_t *info)
{
    static char *buf; /* the ';' command chain buffer */
    static size_t i, j, len;
    char **buf_p = &(info->arg), *p;

    ssize_t r = handle_input(info, &buf, &len);

    if (r == -1)
		return (-1);

    if (len)	/* we have commands left in the chain buffer */
    {
        handle_chain(info, buf, &i, &j, len, &p);

        i = j + 1; /* increment past nulled ';'' */
        if (i >= len) /* reached end of buffer? */
            _RESet_BuFFer_(info, &i, &len);

        *buf_p = p; /* pass back pointer to current command position */

        return (_strlen(p)); /* return length of current command */
    }

    *buf_p = buf; /* else not a chain, pass back buffer from _getline() */
    return (r); /* return length of buffer from _getline() */
}
/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_from_fd(info_t *info, char *buf)
{
	ssize_t r;

	r = read(info->readfd, buf, READ_BUF_SIZE);
	return ((r >= 0) ? r : -1);
}

ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (!*i)
	{
		r = read_from_fd(info, buf);
		*i = (r >= 0) ? r : 0;
	}

	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
ssize_t buffer_reader(info_t *info, char *buf, size_t *len)
{
	ssize_t r = 0;

	if (!*len)
	{
		r = read(info->readfd, buf, READ_BUF_SIZE);
		*len = (r >= 0) ? r : 0;
	}

	return (r);
}

char *locate_newline(char *buf, size_t i)
{
	return (_strchr(buf + i, '\n'));
}

char *memory_allocator(char *p, size_t s, size_t k)
{
	char *new_p;

	new_p = _realloc(p, s, s ? s + k : k + 1);

	if (!new_p) /* MALLOC FAILURE! */
	{
		if (p)
			free(p);
		return (NULL);
	}
	return (new_p);
}

void buffer_copier(char *new_p, char *buf, size_t i, size_t k, size_t s)
{
	s ? _strncat(new_p, buf + i, k - i) : _strncpy(new_p, buf + i, k - i + 1);
}

int _getline(info_t *info, char **ptr, size_t *length)
{
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;
	size_t k;
	static char buf[READ_BUF_SIZE];
	static size_t i, len;

	p = *ptr;

	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = buffer_reader(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = locate_newline(buf, i);
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = memory_allocator(p, s, k);

	if (!new_p)
		return (-1);

	buffer_copier(new_p, buf, i, k, s);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;

	*ptr = p;
	return (s);
}
/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	char *output = "\n$ ";

	if (output)
	{
		do {
			_putchar(*output == '\n' ? '\n' : *output == '$' ? '$' : ' ');
			output++;
		} while (*output);
	}
	_putchar(BUF_FLUSH);
}
