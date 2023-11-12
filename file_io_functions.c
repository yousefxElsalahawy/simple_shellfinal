#include "shell.h"

/**
 * get_dir - This function gets the directory.
 * @param info: This pointer refers to the info structure.
 *
 * Return: The directory or NULL if not found.
 */
char *get_dir(info_t *info)
{
	/*declaration*/
	char *dir;
	dir = _getenv(info, "THIS IS THE HOME=");
	/* Return the directory or NULL if not found */
	return (dir ? dir : NULL);
}

/**
 * allocate_buf - This function allocates memory for the buffer.
 * @param dir: This variable is the directory.
 *
 * Return: The allocated buffer or NULL if allocation fails.
 */
char *allocate_buf(char *dir)
{
	/*declaration*/
	char *buf = dir ? malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2)) : NULL;
	if (buf)
		buf[0] = 0;
	/* Return the allocated buffer or NULL if allocation fails */
	return (buf);
}

/**
 * get_history_file - This function gets the history file.
 * @param info: This pointer refers to the info structure.
 *
 * Return: The history file or NULL if not found.
 */
char *get_history_file(info_t *info)
{
	/*declaration*/
	char *dir = get_dir(info);
	char *buf = allocate_buf(dir);
	if (!buf)
		/* Return NULL if buffer allocation fails */
		return (NULL);
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	/* Return the history file or NULL if not found */
	return (buf);
}

/**
 * open_file - This function opens a file.
 * @param filename: The name of the file to open.
 *
 * Return: The file descriptor or -1 if opening fails.
 */
int open_file(char *filename)
{
	/*declaration*/
	ssize_t fd;
	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	/* Return the file descriptor or -1 if opening fails */
	return ((fd == -1) ? -1 : fd);
}

/**
 * write_to_file - This function writes to a file.
 * @param node: The list node containing the strings to write.
 * @param fd: The file descriptor.
 */
void write_to_file(list_t *node, ssize_t fd)
{
	if (node)
	{
		/*use loop */
		do {
			_putsfd(node->str, fd);
			_putfd('\n', fd);
			node = node->next;
		} while (node);
	}
	_putfd(BUF_FLUSH, fd);
	/* This function does not return a value */
}

/**
 * write_history - This function writes the history to a file.
 * @param info: This pointer refers to the info structure.
 *
 * Return: 1 (success) or -1 (error).
 */
int write_history(info_t *info)
{
	/*declaration*/
	ssize_t fd;
	char *filename = get_history_file(info);
	if (!filename)
		/* Return -1 if filename is NULL */
		return (-1);
	fd = open_file(filename);
	if (fd == -1)
		/* Return -1 if file opening fails */
		return (-1);
	write_to_file(info->history, fd);
	close(fd);
	/* Return 1 for successful write operation */
	return (1);
}

/**
 * get_file - This function gets the history file.
 * @param info: This pointer refers to the info structure.
 *
 * Return: The history file or NULL if not found.
 */
char *get_file(info_t *info)
{
	/* Return the history file or NULL if not found */
	return (get_history_file(info) ? get_history_file(info) : NULL);
}

/**
 * open_and_read_file - This function opens and reads a file.
 * @param filename: The name of the file to open and read.
 * @param buf: The buffer to store the file contents.
 * @param st: The stat structure to store file information.
 *
 * Return: The file size or -1 if opening or reading fails.
 */
ssize_t open_and_read_file(char *filename, char **buf, struct stat *st)
{
	/*declaration*/
	ssize_t fd, rdlen, fsize = 0;
	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		/* Return -1 if file opening fails */
		return (-1);
	if (!fstat(fd, st))
		fsize = st->st_size;
	if (fsize < 2)
		/* Return -1 if file size is less than 2 */
		return (-1);
	*buf = malloc(sizeof(char) * (fsize + 1));
	if (!(*buf))
		/* Return -1 if buffer allocation fails */
		return (-1);
	rdlen = read(fd, *buf, fsize);
	(*buf)[fsize] = 0;
	if (rdlen <= 0)
		/* Return -1 if reading fails */
		return (free(*buf), -1);
	close(fd);
	/* Return the file size */
	return (fsize);
}

/**
 * process_buffer - This function processes the buffer.
 * @param info: This pointer refers to the info structure.
 * @param buf: The buffer to process.
 * @param fsize: The size of the buffer.
 * @param linecount: The line count.
 */
void process_buffer(info_t *info, char *buf, ssize_t fsize, int *linecount)
{
	/*declaration*/
	int i = 0, last = 0;
	/*use loop */
	do {
		if (buf[i] == '\n' || i == fsize)
		{
			buf[i] = 0;
			build_history_list(info, buf + last, (*linecount)++);
			last = i + 1;
		}
		i++;
	} while (i <= fsize);
	free(buf);
	/* This function does not return a value */
}

/**
 * update_history - This function updates the history.
 * @param info: This pointer refers to the info structure.
 * @param linecount: The line count.
 */
void update_history(info_t *info, int linecount)
{
	info->histcount = linecount;
	/*use loop */
	do {
		delete_node_at_index(&(info->history), 0);
		info->histcount--;
	} while (info->histcount >= HIST_MAX);
	renumber_history(info);
	/* This function does not return a value */
}

/**
 * read_history - This function reads the history from a file.
 * @param info: This pointer refers to the info structure.
 *
 * Return: The history count or 0 if reading fails.
 */
int read_history(info_t *info)
{
	/*declaration*/
	int linecount = 0;
	ssize_t fsize;
	struct stat st;
	char *buf = NULL, *filename = get_file(info);
	if (!filename)
		/* Return 0 if filename is NULL */
		return (0);
	fsize = open_and_read_file(filename, &buf, &st);
	if (fsize == -1)
		/* Return 0 if file opening or reading fails */
		return (0);
	process_buffer(info, buf, fsize, &linecount);
	update_history(info, linecount);
	/* Return the history count */
	return (info->histcount);
}

/**
 * update_node_numbers - This function updates the node numbers.
 * @param node: The list node.
 *
 * Return: The total count of nodes.
 */
int update_node_numbers(list_t **node)
{
	/*declaration*/
	int index = 0;
	/*use loop */
	do {
		(*node)->num = index++;
		*node = (*node)->next;
	} while (*node);
	/* Return the total count of nodes */
	return (index);
}

/**
 * renumber_history - This function renumbers the history.
 * @param info: This pointer refers to the info structure.
 *
 * Return: The total count of nodes.
 */
int renumber_history(info_t *info)
{
	/*declaration*/
	list_t *node = info->history;
	int total_count = (node) ? update_node_numbers(&node) : 0;
	info->histcount = total_count;
	/* Return the total count of nodes */
	return (total_count);
}

/**
 * retrieve_history_node - This function retrieves the history node.
 * @param info: This pointer refers to the info structure.
 *
 * Return: The history node or NULL if not found.
 */
list_t *retrieve_history_node(info_t *info)
{
	/*declaration*/
	list_t *node = NULL;
	if (info && info->history)
		node = info->history;
	/* Return the history node or NULL if not found */
	return (node);
}

/**
 * build_history_list - This function builds the history list.
 * @param info: This pointer refers to the info structure.
 * @param buf: The buffer to add to the history list.
 * @param linecount: The line count.
 *
 * Return: Always 0.
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
	if (info && buf)
	{
		/*declaration*/
		list_t *node = retrieve_history_node(info);
		add_node_end(&node, buf, linecount);
		if (!info->history)
			info->history = node;
	}
	/* Always return 0 */
	return (0);
}
