#include "shell.h"


/**
 * check_operator - Checks if current char in buffer is a chain delimeter
 * @info: This pointer refers to the parameter struct
 * @buf: This pointer refers to the char buffer
 * @j: This variable refers to the current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int check_operator(info_t *info, char *buf, size_t *j)
{
	/* Declaration */
	if (!buf)
		return (0);

	/* Use switch */
	switch (buf[*j])
	{
		case '|':
			/* Check for logical OR operator */
			if (buf[*j + 1] == '|')
			{
				buf[*j] = 0;
				(*j)++;
				info->cmd_buf_type = CMD_OR;

				return (1); /* Returns 1 if logical OR operator is found */
			}
			break;
		case '&':
			/* Check for logical AND operator */
			if (buf[*j + 1] == '&')
			{
				buf[*j] = 0;
				(*j)++;
				info->cmd_buf_type = CMD_AND;
				return (1); /* Returns 1 if logical AND operator is found */
			}
			break;
		case ';':
			/* Check for command chain operator */
			buf[*j] = 0;
			info->cmd_buf_type = CMD_CHAIN;
			return (1); /* Returns 1 if command chain operator is found */
		default:
			break;
	}
	return (0); /* Returns 0 if no operator is found */
}

/**
 * is_chain - Checks if the position has changed, indicating an operator was found
 * @info: This pointer refers to the parameter struct
 * @buf: This pointer refers to the char buffer
 * @p: This variable refers to the current position in buf
 *
 * Return: 1 if position has changed, 0 otherwise
 */
int is_chain(info_t *info, char *buf, size_t *p)
{
	/* Declaration */
	size_t j;

	j = *p;

	/* Use loop */
	*p = j + check_operator(info, buf, &j);

	/* Use if */
	return ((*p > j) ? 1 : 0); /* Returns 1 if position has changed, 0 otherwise */
}

/**
 * check_condition - Checks the condition of the command buffer type
 * @info: This pointer refers to the parameter struct
 *
 * Return: 1 if condition is met, 0 otherwise
 */
int check_condition(info_t *info)
{
	return ((info->cmd_buf_type == CMD_AND && info->status) ||
			(info->cmd_buf_type == CMD_OR && !info->status)); /* Returns 1 if condition is met, 0 otherwise */
}

/**
 * update_buffer_and_index - Updates the buffer and index based on the condition
 * @info: This pointer refers to the parameter struct
 * @buf: This pointer refers to the char buffer
 * @index: This variable refers to the current position in buf
 * @len: This variable refers to the length of the buffer
 *
 * Return: Nothing (void function)
 */
void update_buffer_and_index(info_t *info, char *buf,
		size_t *index, size_t len)
{
	/* Declaration */
	int condition = check_condition(info);

	/* Use if */
	buf[*index] = condition ? 0 : buf[*index];

	/* Use loop */
	*index = condition ? len : *index;
}

/**
 * check_chain - Checks the chain and updates the buffer and index
 * @info: This pointer refers to the parameter struct
 * @buf: This pointer refers to the char buffer
 * @p: This variable refers to the current position in buf
 * @i: This variable is unused
 * @len: This variable refers to the length of the buffer
 *
 * Return: Nothing (void function)
 */
void check_chain(info_t *info, char *buf,
		size_t *p, __attribute__((unused)) size_t i, size_t len)
{
	/* Declaration */
	size_t j = *p;

	/* Call the function to update the buffer and index */
	update_buffer_and_index(info, buf, &j, len);

	/* Use loop */
	*p = j;
}














/*****************************************/
/**
 * get_node - Retrieves a node from the alias list
 * @info: This pointer refers to the parameter struct
 *
 * Return: Node if found, NULL otherwise
 */
list_t *get_node(info_t *info)
{
	/* Use if */
	if (!info)
		return (NULL);

	return (node_starts_with(info->alias, info->argv[0], '=')); /* Returns node if found, NULL otherwise */
}

/**
 * get_new_p - Retrieves a new pointer from the node
 * @node: This pointer refers to the node
 *
 * Return: New pointer if found, NULL otherwise
 */
char *get_new_p(list_t *node)
{
	/* Declaration */
	char *p;

	/* Use if */
	if (!node)
		return (NULL);

	p = _strchr(node->str, '=');

	return (p ? _strdup(p + 1) : NULL); /* Returns new pointer if found, NULL otherwise */
}

/**
 * replace_alias - Replaces the alias in the argv[0] with the value from the alias list
 * @info: This pointer refers to the parameter struct
 *
 * Return: 1 if alias is replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
	/* Declaration */
	int i = 0;
	list_t *node;
	char *p;

	/* Use if */
	if (!info)
		return (0);

	/* Use loop */
	do {
		node = get_node(info);

		/* Use if */
		if (!node)
			return (0);

		p = get_new_p(node);

		/* Use if */
		if (!p)
			return (0);
		free(info->argv[0]);
		info->argv[0] = p;
		i++;
	} while (i < 10 && info->argv[0] == NULL);

	return (info->argv[0] != NULL); /* Returns 1 if alias is replaced, 0 otherwise */
}

/**
 * check_variable_type - Checks the type of the variable in argv[i]
 * @info: This pointer refers to the parameter struct
 * @i: This variable refers to the index of argv
 *
 * Return: 0 if not a variable, 1 if status variable, 2 if PID variable, 3 otherwise
 */
int check_variable_type(info_t *info, int i)
{
	/* Use if */
	if (info->argv[i][0] != '$' || !info->argv[i][1])
		return 0;
	else if (!_strcmp(info->argv[i], "$?"))
		return 1;
	else if (!_strcmp(info->argv[i], "$$"))
		return 2;
	else
		return 3; /* Returns 0 if not a variable, 1 if status variable, 2 if PID variable, 3 otherwise */
}

/**
 * replace_string_with_value - Replaces the string in argv[i] with the value
 * @info: This pointer refers to the parameter struct
 * @i: This variable refers to the index of argv
 * @value: This variable refers to the value to replace with
 *
 * Return: Nothing (void function)
 */
void replace_string_with_value(info_t *info, int i, int value)
{
	replace_string(&(info->argv[i]), _strdup(convert_number(value, 10, 0)));
}

/**
 * _get_node_ - Retrieves a node from the environment list
 * @info: This pointer refers to the parameter struct
 * @i: This variable refers to the index of argv
 *
 * Return: Node if found, NULL otherwise
 */
list_t *_get_node_(info_t *info, int i)
{
	return node_starts_with(info->env, &info->argv[i][1], '='); /* Returns node if found, NULL otherwise */
}

/**
 * replace_with_node_value - Replaces the string in argv[i] with the value from the node
 * @info: This pointer refers to the parameter struct
 * @i: This variable refers to the index of argv
 * @node: This pointer refers to the node
 *
 * Return: Nothing (void function)
 */
void replace_with_node_value(info_t *info, int i, list_t *node)
{
	replace_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
}

/**
 * replace_with_empty_string - Replaces the string in argv[i] with an empty string
 * @info: This pointer refers to the parameter struct
 * @i: This variable refers to the index of argv
 *
 * Return: Nothing (void function)
 */
void replace_with_empty_string(info_t *info, int i)
{
	replace_string(&info->argv[i], _strdup(""));
}


/**
 * replace_vars - Replaces the variables in argv with their corresponding values
 * @info: This pointer refers to the parameter struct
 *
 * Return: 0 (void function)
 */
int replace_vars(info_t *info)
{
	/* Declaration */
	int i = 0;
	list_t *node;
	int var_type;

	/* Use loop */
	do {
		var_type = check_variable_type(info, i);

		/* Use switch */
		switch (var_type)
		{
			case 0:
				i++;
				continue;
			case 1:
				replace_string_with_value(info, i, info->status);
				break;
			case 2:
				if (kill(getpid(), 0) == 0)
				{
					replace_string_with_value(info, i, getpid());
				}
				else
				{
					replace_with_empty_string(info, i);
				}
				break;
			case 3:
				node = _get_node_(info, i);
				if (node)
					replace_with_node_value(info, i, node);
				else
					replace_with_empty_string(info, i);
				break;
		}
		i++;
	} while (info->argv[i]);

	return (0); /* Returns 0 (void function) */
}

/**
 * replace_string - Replaces old string with new string
 * @old: This pointer refers to the old string
 * @new: This pointer refers to the new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	/* Use if */
	if (old == NULL || new == NULL)
	{
		_puts("Error: Null pointer passed to replace_string\n");
		return (0); /* Returns 0 if either old or new is NULL */
	}
	if (*old != new)
	{
		free(*old);
		*old = new;
	}
	return (1); /* Returns 1 if replaced */
}
