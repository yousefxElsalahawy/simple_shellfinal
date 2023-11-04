#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int check_operator(info_t *info, char *buf, size_t *j)
{

	if (!buf)
		return (0);


	switch (buf[*j])
	{
		case '|':
			/* Check for logical OR operator */
			if (buf[*j + 1] == '|')
			{
				buf[*j] = 0;
				(*j)++;
				info->cmd_buf_type = CMD_OR;

				return (1);
			}
			break;
		case '&':
			/* Check for logical AND operator */
			if (buf[*j + 1] == '&')
			{
				buf[*j] = 0;
				(*j)++;
				info->cmd_buf_type = CMD_AND;
				return (1);
			}
			break;
		case ';':
			/* Check for command chain operator */
			buf[*j] = 0;
			info->cmd_buf_type = CMD_CHAIN;
			return (1);
		default:
			break;
	}
	return (0);
}

int is_chain(info_t *info, char *buf, size_t *p)
{
	size_t j;

	j = *p;

	*p = j + check_operator(info, buf, &j);

	/* Check if the position has changed, indicating an operator was found */
	return ((*p > j) ? 1 : 0);
}







/**8888888888888888888888888888888888888888*/


int check_condition(info_t *info)
{
	return ((info->cmd_buf_type == CMD_AND && info->status) ||
			(info->cmd_buf_type == CMD_OR && !info->status));
}

void update_buffer_and_index(info_t *info, char *buf,
		size_t *index, size_t len)
{
	int condition = check_condition(info);

	buf[*index] = condition ? 0 : buf[*index];

	*index = condition ? len : *index;
}

void check_chain(info_t *info, char *buf,
		size_t *p, __attribute__((unused)) size_t i, size_t len)
{
	size_t j = *p;

	/* Call the function to update the buffer and index */
	update_buffer_and_index(info, buf, &j, len);

	*p = j;
}

/*****************************************/
list_t *get_node(info_t *info)
{
	if (!info)
		return (NULL);

	return (node_starts_with(info->alias, info->argv[0], '='));
}

char *get_new_p(list_t *node)
{
	char *p;

	if (!node)
		return (NULL);

	p = _strchr(node->str, '=');

	return (p ? _strdup(p + 1) : NULL);
}

int replace_alias(info_t *info)
{
	int i = 0;
	list_t *node;
	char *p;

	if (!info)
		return (0);

	do {
		node = get_node(info);

		if (!node)
			return (0);

		p = get_new_p(node);

		if (!p)
			return (0);
		free(info->argv[0]);
		info->argv[0] = p;
		i++;
	} while (i < 10 && info->argv[0] == NULL);

	return (info->argv[0] != NULL);
}













/**************************************************/

int check_variable_type(info_t *info, int i)
{
    if (info->argv[i][0] != '$' || !info->argv[i][1])
        return 0;
    else if (!_strcmp(info->argv[i], "$?"))
        return 1;
    else if (!_strcmp(info->argv[i], "$$"))
        return 2;
    else
        return 3;
}

void replace_string_with_value(info_t *info, int i, int value)
{
    replace_string(&(info->argv[i]), _strdup(convert_number(value, 10, 0)));
}

list_t *_get_node_(info_t *info, int i)
{
    return node_starts_with(info->env, &info->argv[i][1], '=');
}

void replace_with_node_value(info_t *info, int i, list_t *node)
{
    replace_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
}

void replace_with_empty_string(info_t *info, int i)
{
    replace_string(&info->argv[i], _strdup(""));
}

int replace_vars(info_t *info)
{
    int i = 0;
    list_t *node;
    int var_type;

    do
    {
        var_type = check_variable_type(info, i);

        switch (var_type)
        {
        case 0:
            i++;
            continue;
        case 1:
            replace_string_with_value(info, i, info->status);
            break;
        case 2:
            if (kill(getpid(), 0) == 0) {
                replace_string_with_value(info, i, getpid());
            } else {
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

    return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	if (old == NULL || new == NULL)
	{
		_puts("Error: Null pointer passed to replace_string\n");
		return (0);
	}
	if (*old != new)
	{
		free(*old);
		*old = new;
	}
	return (1);
}





















