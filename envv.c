#include "shell.h"

/**
 * _myenv - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */

int _myenv(info_t *info)
{
    /*Validate the inputs*/
        int result;	

    if (info == NULL || info->env == NULL)
    {
        puts("Error: This is Invalid inputs.");
        return (1);
    }

	 result = print_list_str(info->env);

    if (result != 0)
        puts("Error: Failed to print Environment Variables.");

    /* Return the result*/
    return (result == 0 ? 0 : 1);
}


/**
 * _getenv - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *search_in_list(list_t *node, const char *name)
{
    char *p = NULL;
    list_t *current_node = node;

    while (current_node != NULL)
    {
        p = starts_with(current_node->str, name);
        if (p && *p)
            break;

        current_node = current_node->next;
    }

    return ((p && *p) ? p : NULL);
}

char *_getenv(info_t *info, const char *name)
{
    return ((info != NULL && name != NULL) ? search_in_list(info->env, name) : NULL);
}

/**
 * _mysetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int validate_args(info_t *info)
{
    const int expected_argc = 3;

    return ((info->argc != expected_argc) ? (_eputs("This is Wrong Number of Arguments\n"), 1) : 0);
}



int _mysetenv(info_t *info)
{
    int validation = validate_args(info);

    if (validation)
        return validation;

    return ((_setenv(info, info->argv[1], info->argv[2]) == 0) ? 0 : 1);
}

/**
 * _myunsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int check_argc(info_t *info)
{
    /* If the number of arguments equals 1, print an error message and return 1, otherwise return 0 */
    return ((info->argc == 1) ? (_eputs("Too few arguments.\n"), 1) : 0);
}



int _myunsetenv(info_t *info)
{
    /* Validate the number of arguments */
    int validation = check_argc(info);
        int i;
	
    if (validation)
        return (validation);

      i = 1;
    do {
        _unsetenv(info, info->argv[i]);
        i++;
    } while (i <= info->argc);

    /* Return 0 if successful */
    return 0;
}
/**
 * populate_env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */

list_t *create_env_node()
{
    size_t i = 0;
    list_t *node = NULL;

	/* Create a new node with environment variables */
    if (environ[i])
    {
        do {
            add_node_end(&node, environ[i], 0);

            i++;
        } while (environ[i]);
    }

    return (node);
}

int populate_env_list(info_t *info)
{
    info->env = create_env_node();

    /* Return 0 if successful */
    return ((info->env != NULL) ? 0 : 1);
}
