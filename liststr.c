#include "shell.h"

/**
 * add_node - adds a node to the start of the list
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
list_t *generate_unique_node(void)
{
	list_t *new_node = (list_t *)malloc(sizeof(list_t));

	return ((!new_node) ? NULL :
			(list_t *)_memset((void *)new_node, 0, sizeof(list_t)));
}

void populate_node_data(list_t *node, const char *str, int num)
{
	if (node)
	{
		node->num = num;
		node->str = str ? strdup(str) : NULL;
		if (str && !node->str)
		{
			free(node);
			node = NULL;
		}
	}
}

void link_new_head(list_t **head, list_t *new_head)
{
	if (new_head && head)
	{
		new_head->next = *head;
		*head = new_head;
	}
}

list_t *add_node(list_t **head, const char *str, int num)
{
	if (head)
	{
		list_t *new_head = generate_unique_node();

		if (new_head)
		{
			populate_node_data(new_head, str, num);
			link_new_head(head, new_head);
		}
		return (new_head);
	}
	return (NULL);
}
/**
 * add_node_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
list_t *create_new_node(const char *str, int num)
{
	list_t *new_node = (list_t *)malloc(sizeof(list_t));


	if (new_node == NULL)
		return (NULL);
	new_node->num = num;
	new_node->str = str ? _strdup(str) : NULL;
	if (str && !new_node->str)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

list_t *find_last_node(list_t *node)
{
	if (node)
	{
		while (node->next)
		{
			node = node->next;
		}
	}
	return (node);
}

void link_new_node_to_end(list_t *node, list_t *new_node)
{
	if (node)
	{
		node->next = new_node;
	}
}

list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node = create_new_node(str, num);
	list_t *last_node;

	if (!new_node)
		return (NULL);

	last_node = find_last_node(*head);

	if (last_node)
	{
		link_new_node_to_end(last_node, new_node);
	}
	else
	{
		*head = new_node;
	}
	return (new_node);
}

/**
 * print_list_str - prints only the str element of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
void print_node(const list_t *node)
{
	char *str_to_print = "(nil)";

	if (node && node->str)
		str_to_print = (char *)node->str;

	_puts(str_to_print);
	_puts("\n");
}

size_t print_list_str(const list_t *h)
{

	size_t i = 0;
	const list_t *current_node = h;

	if (!h)
		return (0);
	do {
		print_node(current_node);
		current_node = current_node->next;
		i++;
	} while (current_node);

	return (i);
}
/**
 * delete_node_at_index - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int is_head_null(list_t **head)
{
	return ((!head || !*head) ? 0 : 1);
}

void delete_node(list_t **node)
{
	free((*node)->str);
	free(*node);
	*node = NULL;
}

void delete_head_node(list_t **head)
{
	list_t *temp_node = *head;

	*head = (*head)->next;
	delete_node(&temp_node);
}

void delete_index_node(list_t **node, list_t *prev_node)
{
	prev_node->next = (*node)->next;
	delete_node(node);
}

int iterate_through_nodes(list_t **head, unsigned int index)
{
	list_t *current_node = *head, *prev_node = NULL;
	unsigned int counter = 0;

	if (current_node)
	{
		do {
			if (counter++ == index)
			{
				delete_index_node(&current_node, prev_node);
				return (1);
			}
			prev_node = current_node;
			current_node = current_node->next;
		} while (current_node);
	}
	return (0);
}

int delete_node_at_index(list_t **head, unsigned int index)
{
	if (!is_head_null(head))
		return (0);

	if (index == 0)
	{
		delete_head_node(head);
		return (1);
	}
	else
	{
		return (iterate_through_nodes(head, index));
	}
}
/**
 * free_list - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void annihilate_node(list_t *node)
{
	if (node)
	{
		free(node->str);
		free(node);
	}
}

list_t *fetch_next_in_line(list_t *node)
{
	return (node ? node->next : NULL);
}

void free_list(list_t **head_ptr)
{
	list_t *node, *next_node;

	if (!head_ptr)
		return;

	node = *head_ptr;
	if (node)
	{
		do {
			next_node = fetch_next_in_line(node);
			annihilate_node(node);
			node = next_node;
		} while (node);
	}

	*head_ptr = NULL;
}
