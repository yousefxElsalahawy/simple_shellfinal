#include "shell.h"
/**
 * list_len - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
bool is_list_empty(const list_t *h)
{
	if (h == NULL)
	{
		_puts("Error: List is empty.\n");
		return (true);
	}
	return (false);
}

size_t list_len(const list_t *h)
{
	size_t i = 0;

	if (is_list_empty(h))
		return (0);

	do {
		h = h->next;
		i++;
	} while (h);

	return (i);
}

/**
 * list_to_strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **allocate_string_array(size_t count)
{
	char **result = malloc(sizeof(char *) * (count + 1));

	return (result != NULL ? result : NULL);
}

char *allocate_string(size_t length)
{
	char *result = malloc(length + 1);

	return (result != NULL ? result : NULL);
}

void deallocate_string_array(char **string_array, size_t count)
{
	size_t index = 0;

	do {
		free(string_array[index]);
		index++;
	} while (index < count);

	free(string_array);
}

char **populate_string_array(list_t *node, char **string_array)
{
	size_t index = 0;
	char *string;

	do {
		string = allocate_string(_strlen(node->str));
		if (!string)
		{
			deallocate_string_array(string_array, index);
			return (NULL);
		}
		string = _strcpy(string, node->str);
		string_array[index] = string;
		node = node->next;
		index++;
	} while (node);
	string_array[index] = NULL;

	return (string_array);
}

char **list_to_strings(list_t *head)
{
	size_t count = list_len(head);
	char **string_array;

	string_array = (!head || !count) ? NULL : allocate_string_array(count);
	if (!string_array)
		return (NULL);

	string_array = populate_string_array(head, string_array);
	return (string_array);
}
/**
 * print_list - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
void _Print_The_Node_(const list_t *node, size_t *i)
{
	_puts(convert_number(node->num, 10, 0));
	_putchar(':');
	_putchar(' ');
	_puts(node->str ? node->str : "(nil)");
	_puts("\n");
	(*i)++;
}

size_t traverse_list(const list_t *h, void (*func)(const list_t*, size_t*))
{
	size_t i = 0;
	const list_t *current = h;

	do {
		func(current, &i);
		current = current->next;
	} while (current);

	return (i);
}

size_t print_list(const list_t *h)
{
	size_t i = traverse_list(h, _Print_The_Node_);

	return (i);
}

/**
 * node_starts_with - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
char *check_prefix_and_condition(list_t *node, char *prefix, char c)
{
	char *p = starts_with(node->str, prefix);

	return ((p && ((c == -1) || (*p == c))) ? p : NULL);
}
list_t *node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	if (node != NULL)
	{
		do {
			p = check_prefix_and_condition(node, prefix, c);
			if (p != NULL)
				return (node);
			node = node->next;
		} while (node);
	}
	return (NULL);
}
/**
 * get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t check_node(list_t *head, list_t *node, size_t *i)
{
	do {
		if (head == node)
			return (*i);

		head = head->next;
		(*i)++;
	} while (head != NULL);

	return (-1);
}
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	return ((head == NULL) ? -1 : check_node(head, node, &i));
}
