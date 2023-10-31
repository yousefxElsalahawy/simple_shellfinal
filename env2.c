#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **refresh_environ(info_t *info)
{
    if (info->env)
    {
        info->environ = list_to_strings(info->env);
        info->env_changed = 0;
    }
    return (info->environ);
}

char **get_environ(info_t *info)
{
    char **environ = info->environ;
    int env_changed = info->env_changed;

    return ((!environ || env_changed) ? refresh_environ(info) : environ);
}
/**
 * _unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
char *get_starting_string(list_t *node, char *var)
{
    return (starts_with(node->str, var));
}

int reset_environment(info_t *info, size_t *i, list_t **node)
{
    int env_changed = delete_node_at_index(&(info->env), *i);

    *i = 0;
    *node = info->env;

    return (env_changed);
}

int _unsetenv(info_t *info, char *var)
{
    list_t *node = info->env;
    size_t i = 0;
    char *p;

    if (!node || !var)
        return (0);

    do {
        p = get_starting_string(node, var);

        if (p && *p == '=')
            return (reset_environment(info, &i, &node));

        node = node->next;
        i++;
    } while (node);

    return (info->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
char *create_buf(char *var, char *value)
{
    char *buf = malloc(_strlen(var) + _strlen(value) + 2);

    if (buf)
    {
        _strcpy(buf, var);
        _strcat(buf, "=");
        _strcat(buf, value);
    }
    return (buf);
}

list_t *find_var_in_env(list_t *node, char *var, char *buf, info_t *info)
{
    char *p;

    if (node)
    {
        do {
            p = starts_with(node->str, var);

            if (p && *p == '=')
            {
                free(node->str);
                node->str = buf;
                info->env_changed = 1;
                return (node);
            }
            node = node->next;
        } while (node);
    }
    return (NULL);
}

void add_node_and_free_buf(list_t **env, char *buf, info_t *info)
{
    add_node_end(env, buf, 0);
    free(buf);
    info->env_changed = 1;
}

int _setenv(info_t *info, char *var, char *value)
{
    char *buf;
    list_t *node;

    if (!var || !value)
        return (0);

    buf = create_buf(var, value);
    if (!buf)
        return (1);

    node = find_var_in_env(info->env, var, buf, info);

    return (node ? 0 : (add_node_and_free_buf(&(info->env), buf, info), 0));
}
