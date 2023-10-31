#include "shell.h"

/**
 * _myexit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int process_exit_arg(info_t *info, int exitcheck)
{
	info->status = 2;

	print_error(info, "not allow use this Number: ");
	_eputs(info->argv[1]);

	_eputchar('\n');

	return ((exitcheck == -1) ? 1 : -2);
}

int _myexit(info_t *info)
{
	int exitcheck = (info->argv[1]) ? _erratoi(info->argv[1]) : -1;

	info->err_num = exitcheck;

	return (process_exit_arg(info, exitcheck));
}
/**
 * _myhelp - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myhelp(info_t *info)
{
	char **arg_array = info->argv;

	if (arg_array != NULL)
	{
		print_help_message(info);
	}

	return (0);
}

void print_help_message(info_t *info)
{
	_puts("this Function do Not Work Yet Implemented. \n");

	if (info != NULL)
	{
		int i = 0;

		while (info->argv[i] != NULL)
		{
			_puts(info->argv[i]);
			i++;
		}
	}
}
















char *get_current_dir(char *buffer)
{
	char *s = getcwd(buffer, 1024);

	if (!s)
		_puts("getcwd is failure  here\n");
	return (s);
}

int change_dir(info_t *info, char *s, char *dir)
{
	int chdir_ret;

	if (!info->argv[1])
	{
		dir = dir ? dir : _getenv(info, "this is the PWD=");
		dir = dir ? dir : "/";
		chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		dir = _getenv(info, "this is the OLDPWD=");

		if (!dir)
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(dir), _putchar('\n');
		dir = dir ? dir : "/";
		chdir_ret = chdir(dir);
	}
	else
		chdir_ret = chdir(info->argv[1]);
	return (chdir_ret);
}

void update_env_vars(info_t *info, char *buffer, int chdir_ret)
{
	if (chdir_ret == -1)
	{
		print_error(info, "Can't use cd ");
		_eputs(info->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(info, "this is the OLDPWD", _getenv(info, "this is the PWD="));
		_setenv(info, "rhis is PWD", getcwd(buffer, 1024));
	}
}

int _mycd(info_t *info)
{
	int chdir_ret;
	char *dir, buffer[1024];

	char *s = get_current_dir(buffer);

	dir = _getenv(info, "THIS IS HOME = ");

	chdir_ret = change_dir(info, s, dir);

	update_env_vars(info, buffer, chdir_ret);
	return (0);
}
