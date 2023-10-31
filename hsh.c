#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
void prompt_user(info_t *info)
{
    if (interactive(info))
        _puts("$ ");
    _eputchar(BUF_FLUSH);
}

void _handle_with_input_(info_t *info, char **av, ssize_t *r, int *builtin_ret)
{
    clear_info(info);
    prompt_user(info);
    *r = get_input(info);
    if (*r != -1)
    {
        set_info(info, av);
        *builtin_ret = find_builtin(info);
        if (*builtin_ret == -1)
            find_cmd(info);
    }
    else
        _putchar(interactive(info) ? '\n' : ' ');
    free_info(info, 0);
}

void handle_exit(info_t *info, int builtin_ret)
{
    if (builtin_ret == -2)
    {
        exit(info->err_num == -1 ? info->status : info->err_num);
    }
    if (!interactive(info) && info->status)
        exit(info->status);
}

void cleanup(info_t *info)
{
    free_info(info, 1);
    write_history(info);
}

int hsh(info_t *info, char **av)
{
    ssize_t r = 0;
    int builtin_ret = 0;

    do
    {
        _handle_with_input_(info, av, &r, &builtin_ret);
    } while (r != -1 && builtin_ret != -2);
    
    handle_exit(info, builtin_ret);
    cleanup(info);

    return (builtin_ret);
}
/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
builtin_table* initialize_builtin_table()
{
    static builtin_table builtintbl[] = {
        {"exit", _myexit},
        {"env", _myenv},
        {"help", _myhelp},
        {"history", _myhistory},
        {"setenv", _mysetenv},
        {"unsetenv", _myunsetenv},
        {"cd", _mycd},
        {"alias", _myalias},
        {NULL, NULL}
    };
    return (builtintbl);
}

int execute_builtin(info_t *info, builtin_table *builtintbl, int i)
{
    info->line_count++;
    return (builtintbl[i].func(info));
}

int find_builtin(info_t *info)
{
	int i = 0;
	int built_in_ret = -1;
	builtin_table *builtintbl = initialize_builtin_table();

	while(builtintbl[i].type && built_in_ret == -1)
	{
		built_in_ret = (_strcmp(info->argv[0], builtintbl[i].type) == 0) ? execute_builtin(info, builtintbl, i) : -1;
		i++;
	}
	return (built_in_ret);
}
/**
 * find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void increment_line_count(info_t *info)
{
    info->linecount_flag == 1 ? (info->line_count++, info->linecount_flag = 0) : 0;
}

void count_non_delim(info_t *info, int *k)
{
    int i = 0;

    while (info->arg[i])
    {
        if (!is_delim(info->arg[i], " \t\n"))
            *k += 1;
        i++;
    }
}

void handle_path_found(info_t *info, char *path)
{
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
}

void handle_path_not_found(info_t *info)
{
    if ((interactive(info) || _getenv(info, "PATH=")
        || info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
        fork_cmd(info);
    else if (*(info->arg) != '\n')
    {
        info->status = 127;
        print_error(info, "the command is not found\n");
    }
}

void find_cmd(info_t *info)
{
    char *path = NULL;
    int k = 0, i = 0;

    info->path = info->argv[0];
    increment_line_count(info);
    count_non_delim(info, &k);
    if (!k)
        return;

    do {
        path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
        i++;
    } while (!path && i < 2);

    path ? handle_path_found(info, path) : handle_path_not_found(info);
}
/**
 * fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void handle_fork_error(pid_t child_pid)
{
    child_pid == -1 ? (perror("Error:"), exit(1)) : (void)child_pid;
}

void handle_exec_error(info_t *info)
{
    execve(info->path, info->argv, get_environ(info)) == -1 ? (free_info(info, 1), errno == EACCES ? exit(126) : exit(1)) : (void)info;
}

void handle_child_process(info_t *info)
{
    handle_exec_error(info);
}

void handle_parent_process(info_t *info)
{
    do {
        wait(&(info->status));
    } while (!WIFEXITED(info->status));

    info->status = WEXITSTATUS(info->status);
    info->status == 126 ? print_error(info, "Permission denied\n") : (void)info;
}

void fork_cmd(info_t *info)
{
    pid_t child_pid = fork();
	
    handle_fork_error(child_pid);
    child_pid == 0 ? handle_child_process(info) : handle_parent_process(info);
}
