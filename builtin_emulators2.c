#include "shell.h"

/**
 * _myhistory - This function prints the history of commands
 * @info: This pointer refers to the information structure
 *
 * Return: 0 (success)
 */
int _myhistory(info_t *info)
{
	/*decleration*/
	list_t *temp = info->history;

	print_list(temp);

	return (0);
}

/**
 * find_equal_sign - This function finds the equal sign in a string
 * @str: This variable refers to the string to be searched
 *
 * Return: pointer to the equal sign or NULL
 */
char *find_equal_sign(char *str)
{
	/*decleration*/

	char *p = _strchr(str, '=');


	return (p ? p : NULL);
}

/**
 * delete_alias_node - This function deletes an alias node
 * @info: This pointer refers to the information structure
 * @str: This variable refers to the string to be deleted
 *
 * Return: 1 if node not found, else 0
 */
int delete_alias_node(info_t *info, char *str)
{
	/*decleration*/

	int index = get_node_index(info->alias, node_starts_with
			(info->alias, str, -1));

	return (index >= 0 ? delete_node_at_index(&(info->alias), index) : 1);
}

/**
 * unset_alias - This function unsets an alias
 * @info: This pointer refers to the information structure
 * @str: This variable refers to the string to be unset
 *
 * Return: result of delete_alias_node
 */
int unset_alias(info_t *info, char *str)
{
	/*decleration*/

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

/**
 * find_equal_sign_in_node - This function finds the equal sign in a node
 * @node: This pointer refers to the node to be searched
 *
 * Return: pointer to the equal sign or NULL
 */
char *find_equal_sign_in_node(list_t *node)
{
	return (node ? _strchr(node->str, '=') : NULL);
}

/**
 * print_node_until_char - This function prints a
 * node until a specific character
 * @node: This pointer refers to the node to be printed
 * @p: This variable refers to the specific character
 */
void print_node_until_char(list_t *node, char *p)
{
	/*decleration*/
	char *a = node->str;

	while (a <= p)
	{
		_putchar(*a);
		a++;
	}
}

/**
 * print_alias - This function prints an alias
 * @node: This pointer refers to the node to be printed
 *
 * Return: 0 if successful, 1 if equal sign not found in node
 */
int print_alias(list_t *node)
{
	/*decleration*/
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

/**
 * handle_unset_alias - This function handles the unsetting of an alias
 * @info: This pointer refers to the information structure
 * @str: This variable refers to the string to be unset
 * @p: This variable refers to the pointer to the equal sign
 *
 * Return: result of unset_alias
 */
int handle_unset_alias(info_t *info, char *str, char *p)
{
	/*decleration*/
	int result = 1;

	if (p && !*++p)
	{
		result = unset_alias(info, str);
		unset_alias(info, str);
		if (add_node_end(&(info->alias), str, 0) == NULL)
			result = 0;
	}
	return (result);
}

/**
 * set_alias - This function sets an alias
 * @info: This pointer refers to the information structure
 * @str: This variable refers to the string to be set
 *
 * Return: result of handle_unset_alias
 */
int set_alias(info_t *info, char *str)
{
	return (handle_unset_alias(info, str, _strchr(str, '=')));
}


/**
 * print_all_aliases - This function prints all aliases
 * @info: This pointer refers to the information structure
 */
void print_all_aliases(info_t *info)
{
	/*decleration*/
	list_t *node = info->alias;

	do {
		print_alias(node);
		node = node->next;
	} while (node);
}

/**
 * handle_alias - This function handles an alias
 * @info: This pointer refers to the information structure
 * @arg: This variable refers to the argument
 */
void handle_alias(info_t *info, char *arg)
{
	/*decleration*/
	char *p = _strchr(arg, '=');

	p ? set_alias(info, arg) :
		print_alias(node_starts_with(info->alias, arg, '='));
}

/**
 * _myalias - This function is a mysterious enigma
 * @info: This pointer refers to the information structure
 *
 * Return: 0 (always, because it's a riddle wrapped in a mystery)
 */
int _myalias(info_t *info)
{
	/**
	 * i: This variable refers to the index of the argument vector
	 */
	int i = 1;

	/**
	 * switch: This function uses a switch statement to handle different cases
	 */
	switch (info->argc)
	{
		/**
		 * case 1: This function prints all aliases if there's only one argument
		 */
		case 1:
			print_all_aliases(info);
			break;
			/**
			 * default: This function handles each alias if there are multiple arguments
			 */
		default:
			while (info->argv[i])
			{
				handle_alias(info, info->argv[i]);
				i++;
			}
			break;
	}

	/**
	 * Return: This function always returns 0, because it's an enigma
	 */
	return (0);
}
