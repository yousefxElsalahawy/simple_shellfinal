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







/**888888888888888888888888888888888888888888888888888888888888888888888888888888888888888*/


int check_condition(info_t *info)
{
    return ((info->cmd_buf_type == CMD_AND && info->status) || (info->cmd_buf_type == CMD_OR && !info->status));
}

void update_buffer_and_index(info_t *info, char *buf, size_t *index, size_t len)
{
    int condition = check_condition(info);

    buf[*index] = condition ? 0 : buf[*index];

    *index = condition ? len : *index;
}

void check_chain(info_t *info, char *buf, size_t *p, __attribute__((unused)) size_t i, size_t len)
{
    size_t j = *p;

    /* Call the function to update the buffer and index */
    update_buffer_and_index(info, buf, &j, len);

    *p = j;
}

/***************************************************************************/
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

    do
    {
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













/*************************************************************************************/
typedef void (*replace_func)(info_t *, int);

int is_dollar_sign_present(char *arg)
{
    return (arg[0] == '$' && arg[1]);
}

void replace_status(info_t *info, int i)
{
    if (!_strcmp(info->argv[i], "$?"))
    {
        replace_string(&(info->argv[i]),
            _strdup(convert_number(info->status, 10, 0)));
    }
}

void replace_pid(info_t *info, int i)
{
    if (!_strcmp(info->argv[i], "$$"))
    {
        replace_string(&(info->argv[i]),
            _strdup(convert_number(getpid(), 10, 0)));
    }
}

void replace_env_var(info_t *info, int i)
{
    list_t *node = node_starts_with(info->env, &info->argv[i][1], '=');
    if (node)
    {
        replace_string(&(info->argv[i]),
            _strdup(_strchr(node->str, '=') + 1));
    }
}

void replace_empty(info_t *info, int i)
{
    replace_string(&info->argv[i], _strdup(""));
}

void replace_vars_helper(info_t *info, replace_func funcs[], int funcs_count)
{
    int i = 0, j;

    while (info->argv[i])
    {
        if (is_dollar_sign_present(info->argv[i]))
        {
            for (j = 0; j < funcs_count; j++)
            {
                funcs[j](info, i);
            }
        }
        i++;
    }
}

int replace_vars(info_t *info)
{
    replace_func funcs[] = {replace_status, replace_pid, replace_env_var, replace_empty};
    int funcs_count = sizeof(funcs) / sizeof(funcs[0]);

    replace_vars_helper(info, funcs, funcs_count);

    return (0);
}







/***********************************************************************************/
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
