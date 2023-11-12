#include "shell.h"

/**
 * clear_info_arg - This function clears the arguments in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void clear_info_arg(info_t *info)
{
	info->argv = NULL; /*use if */
	info->arg = NULL;
}

/**
 * clear_info_path_argc - This function clears the path and argument count in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void clear_info_path_argc(info_t *info)
{
	info->argc = 0;
	info->path = NULL;
}

/**
 * clear_info - This function clears all the fields in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void clear_info(info_t *info)
{
	clear_info_path_argc(info);
	clear_info_arg(info);
}

/**
 * set_fname - This function sets the filename in the info struct
 * @info: This pointer refers to the Info Struct
 * @av: This pointer refers to Argument Vector
 *
 * No return value (void function)
 */
void set_fname(info_t *info, char **av)
{
	char *filename = av[0];
	info->fname = filename;
}

/**
 * allocate_argv - This function allocates memory for arguments in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void allocate_argv(info_t *info)
{
	char **arguments = malloc(sizeof(char *) * 2);

	if (arguments) /*use if */
	{
		arguments[0] = _strdup(info->arg);
		arguments[1] = NULL;
	}
	info->argv = arguments;
}

/**
 * set_argv - This function sets the arguments in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void set_argv(info_t *info)
{
	int i = 0;
	char **argv = NULL;

	if (info->arg) /*use if */
	{
		argv = strtow(info->arg, " \t");
		if (!argv) /*use if */
		{
			allocate_argv(info);
		}
		else
		{
			while (argv && argv[i]) /*use loop */
			{
				i++;
			}
			info->argv = argv;
		}
		info->argc = i;
	}
}

/**
 * replace_info - This function replaces aliases and variables in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void replace_info(info_t *info)
{
	replace_alias(info);
	replace_vars(info);
}

/**
 * set_info - This function sets all the fields in the info struct
 * @info: This pointer refers to the Info Struct
 * @av: This pointer refers to Argument Vector
 *
 * No return value (void function)
 */
void set_info(info_t *info, char **av)
{
	set_fname(info, av);
	set_argv(info);
	replace_info(info);
}

/**
 * obliterate_argv - This function frees the arguments in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void obliterate_argv(info_t *info)
{
	ffree(info->argv);
	info->argv = NULL;
}

/**
 * annihilate_path - This function clears the path in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void annihilate_path(info_t *info)
{
	info->path = NULL;

}

/**
 * exterminate_cmd_buf_and_arg - This function frees the command buffer and argument in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void exterminate_cmd_buf_and_arg(info_t *info)
{
	if (!info->cmd_buf) /*use if */
		free(info->arg);
}

/**
 * decimate_lists - This function frees the lists in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void decimate_lists(info_t *info)
{
	int case_num = 1;

	switch (case_num) /* use switch*/
	{
		case 1:
			if (info->env) /*use if */
				free_list(&(info->env));
			/* fall through */
		case 2:
			if (info->history)
				free_list(&(info->history));
			/* fall through */
		case 3:
			if (info->alias) /*use if */
				free_list(&(info->alias));
			break;
	}
}
/**
 * eradicate_environ_and_cmd_buf - This function frees the environment and command buffer in the info struct
 * @info: This pointer refers to the Info Struct
 *
 * No return value (void function)
 */
void eradicate_environ_and_cmd_buf(info_t *info)
{
	ffree(info->environ);
	info->environ = NULL;
	bfree((void **)info->cmd_buf);

	if (info->readfd > 2) /*use if */
		close(info->readfd);
	_putchar(BUF_FLUSH);
}

/**
 * free_info - This function frees all the fields in the info struct
 * @info: This pointer refers to the Info Struct
 * @all: This variable is used to determine if all fields should be freed
 *
 * No return value (void function)
 */
void free_info(info_t *info, int all)
{
	obliterate_argv(info);
	annihilate_path(info);
	if (all) /*use if */
	{
		exterminate_cmd_buf_and_arg(info);
		decimate_lists(info);
		eradicate_environ_and_cmd_buf(info);
	}
	/* The function returns here */
}
