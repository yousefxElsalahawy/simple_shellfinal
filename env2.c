#include "shell.h"

/**
 * refresh_environ - This function refreshes the environment
 * @info: This pointer refers to the Info Struct
 *
 * Return: info->environ (success)
 */
char **refresh_environ(info_t *info)
{
	/* Check if info->env is not NULL */
	if (info->env)
	{
		/* Convert list to strings */
		info->environ = list_to_strings(info->env);
		/* Reset env_changed flag */
		info->env_changed = 0;
	}
	/* Return the refreshed environment */
	return (info->environ);
}

/**
 * get_environ - This function gets the environment
 * @info: This pointer refers to the Info Struct
 *
 * Return: refresh_environ(info) or environ (success)
 */
char **get_environ(info_t *info)
{
	/* Check if info->env is not NULL */
	if (info->env)
	{
		/* Convert list to strings */
		info->environ = list_to_strings(info->env);
		/* Reset env_changed flag */
		info->env_changed = 0;
	}
	/* Return the refreshed environment */
	return (info->environ);
}

/**
 * get_starting_string - This function gets the starting string
 * @node: This pointer refers to the Node
 * @var: This variable is used for some specific purpose
 *
 * Return: starts_with(node->str, var) (success)
 */
char *get_starting_string(list_t *node, char *var)
{
	/* Return the result of starts_with function */
	return (starts_with(node->str, var));
}

/**
 * reset_environment - This function resets the environment
 * @info: This pointer refers to the Info Struct
 * @i: This variable is used for some specific purpose
 * @node: This pointer refers to the Node
 *
 * Return: env_changed (success)
 */
int reset_environment(info_t *info, size_t *i, list_t **node)
{
	/* Delete node at index i and return the result */
	int env_changed = delete_node_at_index(&(info->env), *i);

	/* Reset i and node */
	*i = 0;
	*node = info->env;

	/* Return the result of env_changed */
	return (env_changed);
}

/**
 * _unsetenv - This function unsets the environment variable
 * @info: This pointer refers to the Info Struct
 * @var: This variable is used for some specific purpose
 *
 * Return: reset_environment(info, &i, &node) or info->env_changed (success)
 */
int _unsetenv(info_t *info, char *var)
{
	/* Initialize node and i */
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	/* Check if node or var is NULL */
	if (!node || !var)
		return (0);

	/* Loop through the nodes */
	do {
		/* Get the starting string */
		p = get_starting_string(node, var);

		/* Check if p is not NULL and the first character is '=' */
		if (p && *p == '=')
			/* Reset the environment and return the result */
			return (reset_environment(info, &i, &node));

		/* Move to the next node and increment i */
		node = node->next;
		i++;
	} while (node);

	/* Return the result of env_changed */
	return (info->env_changed);
}

/**
 * create_buf - This function creates a buffer
 * @var: This variable is used for some specific purpose
 * @value: This variable is used for some specific purpose
 *
 * Return: buf (success)
 */
char *create_buf(char *var, char *value)
{
	/* Allocate memory for buf */
	char *buf = malloc(_strlen(var) + _strlen(value) + 2);

	/* Check if buf is not NULL */
	if (buf)
	{
		/* Copy var to buf */
		_strcpy(buf, var);
		/* Concatenate '=' to buf */
		_strcat(buf, "=");
		/* Concatenate value to buf */
		_strcat(buf, value);
	}
	/* Return buf */
	return (buf);
}

/**
 * find_var_in_env - This function finds a variable in the environment
 * @node: This pointer refers to the Node
 * @var: This variable is used for some specific purpose
 * @buf: This variable is used for some specific purpose
 * @info: This pointer refers to the Info Struct
 *
 * Return: node or NULL (success)
 */
list_t *find_var_in_env(list_t *node, char *var, char *buf, info_t *info)
{
	char *p;

	/* Check if node is not NULL */
	if (node)
	{
		/* Loop through the nodes */
		do {
			/* Get the starting string */
			p = starts_with(node->str, var);

			/* Check if p is not NULL and the first character is '=' */
			if (p && *p == '=')
			{
				/* Free node->str */
				free(node->str);
				/* Assign buf to node->str */
				node->str = buf;
				/* Set env_changed to 1 */
				info->env_changed = 1;
				/* Return node */
				return (node);
			}
			/* Move to the next node */
			node = node->next;
		} while (node);
	}
	/* Return NULL */
	return (NULL);
}

/**
 * add_node_and_free_buf - This function adds a node and frees the buffer
 * @env: This pointer refers to the Environment
 * @buf: This variable is used for some specific purpose
 * @info: This pointer refers to the Info Struct
 *
 * Return: None
 */
void add_node_and_free_buf(list_t **env, char *buf, info_t *info)
{
	/* Add node to the end of env */
	add_node_end(env, buf, 0);
	/* Free buf */
	free(buf);
	/* Set env_changed to 1 */
	info->env_changed = 1;
}

/**
 * _setenv - This function sets the environment variable
 * @info: This pointer refers to the Info Struct
 * @var: This variable is used for some specific purpose
 * @value: This variable is used for some specific purpose
 *
 * Return: 0 (success), 1 (error)
 */
int _setenv(info_t *info, char *var, char *value)
{
	/* Initialize buf */
	char *buf;
	/* Initialize node */
	list_t *node;

	/* Check if var or value is NULL */
	if (!var || !value)
		return (0);

	/* Create buf */
	buf = create_buf(var, value);
	/* Check if buf is NULL */
	if (!buf)
		return (1);

	/* Find var in env */
	node = find_var_in_env(info->env, var, buf, info);

	/* Check if node is not NULL */
	return (node ? 0 : (add_node_and_free_buf(&(info->env), buf, info), 0));
}

