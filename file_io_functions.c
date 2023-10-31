#include "shell.h"

/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_dir(info_t *info)
{
    char *dir;

	dir = _getenv(info, "THIS IS THE HOME=");
    return (dir ? dir : NULL);
}

char *allocate_buf(char *dir)
{
    char *buf = dir ? malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2)) : NULL;

    if (buf)
        buf[0] = 0;

    return (buf);
}

char *get_history_file(info_t *info)
{
    char *dir = get_dir(info);
    char *buf = allocate_buf(dir);

    if (!buf)
        return (NULL);

    _strcpy(buf, dir);
    _strcat(buf, "/");
    _strcat(buf, HIST_FILE);

    return (buf);
}
/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int open_file(char *filename)
{
    ssize_t fd;

    fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);

    free(filename);

    return ((fd == -1) ? -1 : fd);

}

void write_to_file(list_t *node, ssize_t fd)
{
    if (node)
    {
        do {
            _putsfd(node->str, fd);

            _putfd('\n', fd);

            node = node->next;
        } while (node);
    }
    _putfd(BUF_FLUSH, fd);

}

int write_history(info_t *info)
{
    ssize_t fd;

    char *filename = get_history_file(info);

    if (!filename)
        return (-1);

    fd = open_file(filename);
    if (fd == -1)
        return (-1);

    write_to_file(info->history, fd);

    close(fd);
    return (1);
}
/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
char *get_file(info_t *info)
{
    return get_history_file(info) ? get_history_file(info) : NULL;
}

ssize_t open_and_read_file(char *filename, char **buf, struct stat *st)
{
    ssize_t fd, rdlen, fsize = 0;

    fd = open(filename, O_RDONLY);
    free(filename);

    if (fd == -1)
        return (-1);

    if (!fstat(fd, st))
        fsize = st->st_size;

    if (fsize < 2)
        return (-1);
    *buf = malloc(sizeof(char) * (fsize + 1));
    if (!(*buf))
        return (-1);
    rdlen = read(fd, *buf, fsize);
    (*buf)[fsize] = 0;
    if (rdlen <= 0)
        return (free(*buf), -1);
    close(fd);
    return fsize;
}

void process_buffer(info_t *info, char *buf, ssize_t fsize, int *linecount)
{
    int i = 0, last = 0;

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
}

void update_history(info_t *info, int linecount)
{
    info->histcount = linecount;
    do {
        delete_node_at_index(&(info->history), 0);
        info->histcount--;
    } while (info->histcount >= HIST_MAX);
    renumber_history(info);
}

int read_history(info_t *info)
{
    int linecount = 0;
    ssize_t fsize;
    struct stat st;
    char *buf = NULL, *filename = get_file(info);

    if (!filename)
        return (0);

    fsize = open_and_read_file(filename, &buf, &st);
    if (fsize == -1)
        return (0);

    process_buffer(info, buf, fsize, &linecount);
    update_history(info, linecount);

    return (info->histcount);
}
/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int update_node_numbers(list_t **node)
{
    int index = 0;

    do
    {
        (*node)->num = index++;
        *node = (*node)->next;
    } while (*node);
    return (index);
}

int renumber_history(info_t *info)
{
    list_t *node = info->history;

    int total_count = (node) ? update_node_numbers(&node) : 0;

    info->histcount = total_count;
    return (total_count);
}


/*****************************************************************************************/
list_t *retrieve_history_node(info_t *info)
{
    list_t *node = NULL;

    if (info && info->history)
        node = info->history;

    return (node);
}

int build_history_list(info_t *info, char *buf, int linecount)
{
    if (info && buf)
    {
        list_t *node = retrieve_history_node(info);

        add_node_end(&node, buf, linecount);

        if (!info->history)
            info->history = node;
    }

    return (0);
}
