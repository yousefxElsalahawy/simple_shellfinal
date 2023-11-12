#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */

int check_path(char *path, struct stat *st)
{
	int result = (!path || stat(path, st)) ? 0 : 1;

	return (result);
}

int check_file_mode(struct stat *st)
{
	int result;

	if (st->st_mode & S_IFREG)
		result = 1;
	else
		result = 0;

	return (result);
}

int is_cmd(info_t *info, char *path)
{
	struct stat st;
	int path_check, mode_check;

	(void)info;
	path_check = check_path(path, &st);
	if (path_check == 0)
		mode_check = 0;
	else
		mode_check = check_file_mode(&st);

	return (mode_check);
}
/**
 * dup_chars - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *_COpYY_cHaRS_(char *pathstr, int start, int stop, char *buf)
{
	int index = start, buffer_index = 0;

	do {
		if (pathstr[index] != ':') {
			buf[buffer_index] = pathstr[index];
			buffer_index++;
		}
		index++;
	} while (index < stop);

	buf[buffer_index] = '\0';
	return (buf);
}

char *dup_chars(char *pathstr, int start, int stop)
{
	static char buffer[1024];

	return (_COpYY_cHaRS_(pathstr, start, stop, buffer));
}

/**
 * find_path - finds this cmd in the PATH string
 * @info: the info struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */

char *check_cmd(info_t *info, char *cmd)
{
	return (((_strlen(cmd) > 2) && starts_with(cmd, "./") && is_cmd(info, cmd)) ? cmd : NULL);
}

char *build_path(char *path, char *cmd)
{
	if (!*path)
		_strcat(path, cmd);
	else {
		_strcat(path, "/");
		_strcat(path, cmd);
	}
	return (path);
}
char *find_cmd_in_path(info_t *info, char *pathstr, char *cmd, int i, int curr_pos)
{
	char *path;

	do {
		if (!pathstr[i] || pathstr[i] == ':') {
			path = dup_chars(pathstr, curr_pos, i);
			path = build_path(path, cmd);
			if (is_cmd(info, path))
				return (path);
			curr_pos = i;
		}
		i++;
	} while (pathstr[i]);
	return (NULL);
}

char *find_path(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path = check_cmd(info, cmd);

	return ((path) ? path : (pathstr ? find_cmd_in_path(info, pathstr, cmd, i, curr_pos) : NULL));
}
