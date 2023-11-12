#include "shell.h"

/**
 * alien_translator - This function translates alien language
 * @str: This variable refers to the alien string to be translated
 *
 * Return: Translated integer value
 */
int alien_translator(char *str)
{
	return (atoi(str));
}

/**
 * interstellar_communicator - This function communicates interstellar messages
 * @info: This pointer refers to the information structure
 * @message: This variable refers to the message to be communicated
 */
void interstellar_communicator(info_t *info, char *message)
{
	info->status = 2;
	_eputs(message);
	_eputs(info->argv[1]);
	_eputchar('\n');
}

/**
 * galactic_checker - This function checks galactic conditions
 * @info: This pointer refers to the information structure
 *
 * Return: Exit check status
 */
int galactic_checker(info_t *info)
{
	int exitcheck = alien_translator(info->argv[1]);

	return ((exitcheck == -1) ? (interstellar_communicator(info, "Illegal number: "), 1) : exitcheck);
}

/**
 * cosmic_status_setter - This function sets cosmic status
 * @info: This pointer refers to the information structure
 * @exitcheck: This variable refers to the exit check status
 */
void cosmic_status_setter(info_t *info, int exitcheck)
{
	info->err_num = exitcheck;
}

/**
 * _myexit - This function exits the shell
 * @info: This pointer refers to the information structure
 *
 * Return: Exit status
 */
int _myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit argument */
	{
		exitcheck = galactic_checker(info);
		switch (exitcheck) {
			case 1:
				return (1);
			default:
				cosmic_status_setter(info, exitcheck);
				return (-2);
		}
	}
	else
	{
		info->err_num = -1;
		return (-2);
	}
}

/**
 * _myhelp - This function provides help
 * @info: This pointer refers to the information structure
 *
 * Return: Always 0
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

/**
 * print_help_message - This function prints help message
 * @info: This pointer refers to the information structure
 */
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

/**
 * get_current_dir - This function gets the current directory
 * @buffer: This variable refers to the buffer for the directory path
 *
 * Return: Current directory path
 */
char *get_current_dir(char *buffer)
{
	char *s = getcwd(buffer, 1024);

	if (!s)
		_puts("getcwd is failure  here\n");
	return (s);
}

/**
 * change_dir - This function changes the directory
 * @info: This pointer refers to the information structure
 * @s: This variable refers to the current directory path
 * @dir: This variable refers to the directory to change to
 *
 * Return: Change directory status
 */
int change_dir(info_t *info, char *s, char *dir)
{
	/*decleration*/
	int chdir_ret;

	/*use if */
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











/**
 * update_env_vars - This function updates environment variables
 * @info: This pointer refers to the information structure
 * @buffer: This variable refers to the buffer for the directory path
 * @chdir_ret: This variable refers to the change directory status
 */
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
/**
 * _mycd - This function changes the current directory
 * @info: This pointer refers to the information structure
 *
 * Return: Always 0
 */
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

