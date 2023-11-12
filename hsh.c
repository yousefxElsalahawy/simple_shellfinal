#include "shell.h"

/**
 * prompt_user - This function prompts the user for input
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void prompt_user(info_t *info)
{
	if (interactive(info)) /*use if */
		_puts("$ ");
	_eputchar(BUF_FLUSH);
}

/**
 * _handle_with_input_ - This function handles user input
 * @info: This pointer refers to parameter and Info Struct
 * @av: This pointer refers to Argument
 * @r: This pointer refers to read value
 * @builtin_ret: This pointer refers to builtin return value
 *
 * Return: void
 */
void _handle_with_input_(info_t *info, char **av, ssize_t *r, int *builtin_ret)
{
	clear_info(info);
	prompt_user(info);
	*r = get_input(info);
	if (*r != -1) /*use if */
	{
		set_info(info, av);
		*builtin_ret = find_builtin(info);
		if (*builtin_ret == -1) /*use if */
			find_cmd(info);
	}
	else
		_putchar(interactive(info) ? '\n' : ' ');
	free_info(info, 0);
}

/**
 * handle_exit - This function handles exit scenarios
 * @info: This pointer refers to parameter and Info Struct
 * @builtin_ret: This is the return value of the builtin function
 *
 * Return: void
 */
void handle_exit(info_t *info, int builtin_ret)
{
	if (builtin_ret == -2) /*use if */
	{
		exit(info->err_num == -1 ? info->status : info->err_num);
	}
	if (!interactive(info) && info->status) /*use if */
		exit(info->status);
}

/**
 * cleanup - This function cleans up the info struct
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void cleanup(info_t *info)
{
	free_info(info, 1);
	write_history(info);
}

/**
 * hsh - This function is the main shell loop
 * @info: This pointer refers to parameter and Info Struct
 * @av: This pointer refers to Argument
 *
 * Return: builtin_ret (success or error code)
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	do { /*use loop */
		_handle_with_input_(info, av, &r, &builtin_ret);
	} while (r != -1 && builtin_ret != -2);

	handle_exit(info, builtin_ret);
	cleanup(info);

	return (builtin_ret); /* Returns the return value of the builtin function */
}

/**
 * initialize_builtin_table - This function initializes the builtin table
 *
 * Return: builtintbl (pointer to the builtin table)
 */
builtin_table *initialize_builtin_table(void)
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
	return (builtintbl); /* Returns a pointer to the builtin table */
}

/**
 * execute_builtin - This function executes a builtin command
 * @info: This pointer refers to parameter and Info Struct
 * @builtintbl: This pointer refers to the builtin table
 * @i: This is the index of the builtin command in the table
 *
 * Return: builtintbl[i].func(info) (success or error code)
 */
int execute_builtin(info_t *info, builtin_table *builtintbl, int i)
{
	info->line_count++;
	return (builtintbl[i].func(info)); /* Returns the return value of the builtin function */
}

/**
 * find_builtin - This function finds a builtin command
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: built_in_ret (success or error code)
 */
int find_builtin(info_t *info)
{
	int i = 0;
	int built_in_ret = -1;
	builtin_table *builtintbl = initialize_builtin_table();

	while (builtintbl[i].type && built_in_ret == -1) /*use loop */
	{
		built_in_ret = (_strcmp(info->argv[0],
					builtintbl[i].type) == 0) ?
			execute_builtin(info, builtintbl, i) : -1;
		i++;
	}
	return (built_in_ret); /* Returns the return value of the builtin function */
}


/**
 * increment_line_count - This function increments the line count
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void increment_line_count(info_t *info)
{
	info->linecount_flag == 1 ? (info->line_count++,
			info->linecount_flag = 0) : 0;
}

/**
 * count_non_delim - This function counts non-delimiter characters
 * @info: This pointer refers to parameter and Info Struct
 * @k: This pointer refers to the count of non-delimiter characters
 *
 * Return: void
 */
void count_non_delim(info_t *info, int *k)
{
	int i = 0;

	while (info->arg[i]) /*use loop */
	{
		if (!is_delim(info->arg[i], " \t\n")) /*use if */
			*k += 1;
		i++;
	}
}

/**
 * handle_path_found - This function handles the scenario when a path is found
 * @info: This pointer refers to parameter and Info Struct
 * @path: This pointer refers to the found path
 *
 * Return: void
 */
void handle_path_found(info_t *info, char *path)
{
	if (path) /*use if */
	{
		info->path = path;
		fork_cmd(info);
	}
}

/**
 * handle_path_not_found - This function handles the scenario when a path is not found
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void handle_path_not_found(info_t *info)
{
	/*usef if*/
	if ((interactive(info) || _getenv(info, "PATH=")
				|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0])) /*use if */
		fork_cmd(info);
	else if (*(info->arg) != '\n') /*use if */
	{
		info->status = 127;
		print_error(info, "the command is not found\n");
	}
}

/**
 * find_cmd - This function finds a command in PATH
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int k = 0, i = 0;

	info->path = info->argv[0];
	increment_line_count(info);
	count_non_delim(info, &k);
	if (!k) /*use if */
		return;

	do { /*use loop */
		path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
		i++;
	} while (!path && i < 2); /*use loop */

	path ? handle_path_found(info, path) : handle_path_not_found(info);
}

/**
 * handle_fork_error - This function handles fork errors
 * @child_pid: This is the process ID of the child process
 *
 * Return: void
 */
void handle_fork_error(pid_t child_pid)
{
	child_pid == -1 ? (perror("Error:"), exit(1)) : (void)child_pid;
}

/**
 * handle_exec_error - This function handles exec errors
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void handle_exec_error(info_t *info)
{
	execve(info->path, info->argv, get_environ(info)) == -1 ?
		(free_info(info, 1), errno == EACCES ?
		 exit(126) : exit(1)) : (void)info;
}

/**
 * handle_child_process - This function handles child process scenarios
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void handle_child_process(info_t *info)
{
	handle_exec_error(info);
}

/**
 * handle_parent_process - This function handles parent process scenarios
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void handle_parent_process(info_t *info)
{
	do { /*use loop */
		wait(&(info->status));
	} while (!WIFEXITED(info->status));

	info->status = WEXITSTATUS(info->status);
	info->status == 126 ? print_error(info, "Permission denied\n") : (void)info;
}

/**
 * fork_cmd - This function forks a an exec thread to run cmd
 * @info: This pointer refers to parameter and Info Struct
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
	/*decleration*/
	pid_t child_pid = fork();

	handle_fork_error(child_pid);
	child_pid == 0 ? handle_child_process(info) : handle_parent_process(info);
}
