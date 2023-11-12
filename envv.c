#include "shell.h"

/**
 * validate_inputs - This function validates the inputs
 * @info: This pointer refers to the Info Struct
 *
 * Return: 0 (success), 1 (error)
 */
int validate_inputs(info_t *info)
{
	if (info == NULL || info->env == NULL)
	{
		puts("Error: This is Invalid inputs.");
		return (1); /* Return 1 if inputs are invalid */
	}
	return (0); /* Return 0 if inputs are valid */
}

/**
 * print_env - This function prints the environment
 * @info: This pointer refers to the Info Struct
 *
 * Return: result of the print operation
 */
int print_env(info_t *info)
{
	int result = print_list_str(info->env);
	if (result != 0)
	{
		puts("Error: Failed to print Environment Variables.");
	}
	return result; /* Return the result of the print operation */
}

/**
 * _myenv - This function handles the myenv command
 * @info: This pointer refers to the Info Struct
 *
 * Return: 0 (success), 1 (error)
 */
int _myenv(info_t *info)
{
	int printResult = print_env(info);
	int validationResult = validate_inputs(info);
	if (validationResult != 0)
	{
		return validationResult; /* Return the result ofvalidation */
	}


	return (printResult == 0 ? 0 : 1); /* Return result of print */
}




/**
 * strange_starts - This function checks if a string starts with a specific name
 * @current_node: This pointer refers to the current node in the list
 * @name: This variable is used to specify the name to check
 *
 * Return: pointer to the start of the string if it starts with the name, NULL otherwise
 */
char *strange_starts(list_t *current_node, const char *name)
{
	char *p = starts_with(current_node->str, name);
	return ((p && *p) ? p : NULL);
	/* Return pointer to the start of the string if it starts with the name, NULL otherwise */
}

/**
 * innovative_search - This function performs a search in the list
 * @node: This pointer refers to the node in the list
 * @name: This variable is used to specify the name to search for
 *
 * Return: pointer to the found element, NULL if not found
 */
char *innovative_search(list_t *node, const char *name)
{
	char *p = NULL;
	list_t *current_node = node;

	while (current_node != NULL) /*use loop */
	{
		p = strange_starts(current_node, name);
		if (p)
			break;

		current_node = current_node->next;
	}

	return (p); /* Return pointer to the found element, NULL if not found */
}

/**
 * _getenv - This function gets the environment variable
 * @info: This pointer refers to the Info Struct
 * @name: This variable is used to specify the name of the environment variable
 *
 * Return: pointer to the environment variable, NULL if not found
 */
char *_getenv(info_t *info, const char *name)
{
	return ((info != NULL && name != NULL) ?
			innovative_search(info->env, name) : NULL); /* Return pointer to the environment variable, NULL if not found */
}




/**
 * validate_args - This function validates the arguments
 * @info: This pointer refers to the Info Struct
 *
 * Return: 0 (success), 1 (error)
 */
int validate_args(info_t *info)
{
	const int expected_argc = 3;

	return ((info->argc != expected_argc) ?
			(_eputs("This is Wrong Number of Arguments\n"), 1) : 0); /* Return 0 if the number of arguments is correct, 1 otherwise */
}

/**
 * _mysetenv - This function sets the environment variable
 * @info: This pointer refers to the Info Struct
 *
 * Return: 0 (success), 1 (error)
 */
int _mysetenv(info_t *info)
{
	int validation = validate_args(info);

	if (validation)
		return (validation); /* Return the result of the validation */

	return ((_setenv(info, info->argv[1], info->argv[2]) == 0) ? 0 : 1); /* Return 0 if the environment variable is set successfully, 1 otherwise */
}

/**
 * check_argc - This function checks the number of arguments
 * @info: This pointer refers to the Info Struct
 *
 * Return: 0 (success), 1 (error)
 */
int check_argc(info_t *info)
{
	return ((info->argc == 1) ? (_eputs("Too few arguments.\n"), 1) : 0); /* Return 0 if the number of arguments is correct, 1 otherwise */
}



/**
 * _myunsetenv - This function unsets the environment variable
 * @info: This pointer refers to the Info Struct
 *
 * Return: 0 (success), 1 (error)
 */
int _myunsetenv(info_t *info)
{
	int validation = check_argc(info);
	int i;

	if (validation)
		return (validation); /* Return the result of the validation */

	i = 1;
	do {
		_unsetenv(info, info->argv[i]);
		i++;
	} while (i <= info->argc);

	return (0); /* Return 0 if the environment variable is unset successfully */
}

/**
 * create_env_node - This function creates a new environment node
 *
 * Return: pointer to the new node
 */
list_t *create_env_node(void)
{
	size_t i = 0;
	list_t *node = NULL;

	if (environ[i])
	{
		do {
			add_node_end(&node, environ[i], 0);
			i++;
		} while (environ[i]);
	}

	return (node); /* Return pointer to the new node */
}

/**
 * populate_env_list - This function populates the environment list
 * @info: This pointer refers to the Info Struct
 *
 * Return: 0 (success), 1 (error)
 */
int populate_env_list(info_t *info)
{
	info->env = create_env_node();

	return ((info->env != NULL) ? 0 : 1); /* Return 0 if the environment list is populated successfully, 1 otherwise */
}
