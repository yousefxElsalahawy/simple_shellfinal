#include "shell.h"

int _myhistory(info_t *info)
{
	list_t *temp = info->history;

    print_list(temp);

    return (0);
}
/***************************************************************************/




char* find_equal_sign(char *str)
{
    char *p = _strchr(str, '=');

    return (p ? p : NULL);
}

int delete_alias_node(info_t *info, char *str)
{
    int index = get_node_index(info->alias, node_starts_with(info->alias, str, -1));

    return (index >= 0 ? delete_node_at_index(&(info->alias), index) : 1);
}

int unset_alias(info_t *info, char *str)
{
    int ret;
    char *p, c;

    p = find_equal_sign(str);

    if (!p)
        return (1);
    c = *p;
    *p = 0;

    do {
        ret = delete_alias_node(info, str);
    } while (ret == 0);
    *p = c;

    return (ret);
}











#include "shell.h"


char* find_equal_sign_in_node(list_t *node)
{
    return (node ? _strchr(node->str, '=') : NULL);
}

void print_node_until_char(list_t *node, char *p)
{
    char *a = node->str;

    while (a <= p)
    {
        _putchar(*a);
        a++;
    }
}

int print_alias(list_t *node)
{
    char *p = find_equal_sign_in_node(node);

    if (p)
    {
        print_node_until_char(node, p);
        _putchar('\'');

        _puts(p + 1);

        _puts("'\n");

        return (0);
    }
    return (1);
}



/***************************************************************************/




int handle_unset_alias(info_t *info, char *str, char *p)
{
	int result = 1;

	if (p && !*++p)
	{
		result = unset_alias(info, str);
		unset_alias(info, str);

		if (add_node_end(&(info->alias), str, 0) == NULL)
		{
			result = 0;
		}
	}
	return (result);
}

int set_alias(info_t *info, char *str)
{
	return (handle_unset_alias(info, str, _strchr(str, '=')));
}

















#include "shell.h"



void print_all_aliases(info_t *info)
{
	list_t *node = info->alias;

	do
	{
		print_alias(node);

		node = node->next;
	} while (node);
}

void handle_alias(info_t *info, char *arg)
{
	char *p = _strchr(arg, '=');

	p ? set_alias(info, arg) : print_alias(node_starts_with(info->alias, arg, '='));
}

int _myalias(info_t *info)
{
	int i = 1;

	switch (info->argc)
	{
	case 1:
		print_all_aliases(info);
		break;
	default:
		while (info->argv[i])
		{
			handle_alias(info, info->argv[i]);
			i++;
		}
		break;
	}

	return (0);
}
