#include "shell.h"

/**
 * bfree - frees a pointer and NULLs the address
 * @ptr: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int is_null(void **ptr)
{
    return (!ptr || !*ptr ? 1 : 0);
}

void free_and_nullify(void **ptr)
{
    free(*ptr);
    *ptr = NULL;
}

int bfree(void **ptr)
{
    return (is_null(ptr) ? 0 : (free_and_nullify(ptr), 1));
}

