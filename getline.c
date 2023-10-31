#include "shell.h"



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

void handle_chain(info_t *info, char *buf, size_t *i, size_t len)
{
	size_t j = *i;

	check_chain(info, buf, &j, *i, len);

	do {

		if (is_chain(info, buf, &j))
			break;
		j++;
	} while (j < len);

	*i = j;
}

void handle_buffer_end(info_t *info, size_t *i, size_t *len)
{
	*i = *i + 1;

	if (*i >= *len)
	{
		*i = *len = 0;
		info->cmd_buf_type = CMD_NORM;
	}
}

ssize_t get_input(info_t *info)
{
	static char *buf;
	static size_t i, len;
	char **buf_p = &(info->arg), *p;
	ssize_t r;

	r = handle_input(info, &buf, &len);

	if (r == -1)
		return (-1);

	switch (len)
	{
		case 0:
			*buf_p = buf;
			return (r);
		default:
			handle_chain(info, buf, &i, len);
			handle_buffer_end(info, &i, &len);

			p = buf + i;
			*buf_p = p;
			return (_strlen(p));
	}
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
