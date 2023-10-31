#include "shell.h"

/**
 **_memset - fills memory with a constant byte
 *@s: the pointer to the memory area
 *@b: the byte to fill *s with
 *@n: the amount of bytes to be filled
 *Return: (s) a pointer to the memory area s
 */
void fillMemory(char *s, char b, unsigned int n, unsigned int i)
{
	do {
		s[i] = b;
		i++;
	} while (i < n);
}

char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i = 0;

	if (n > 0)
		fillMemory(s, b, n, i);

	return (s);
}
/**
 * ffree - frees a string of strings
 * @pp: string of strings
 */
void freeArray(char **pp, int i)
{
    for (; pp[i] != NULL; i++)
        free(pp[i]);
}

void ffree(char **pp)
{
    char **a = pp;
    int i = 0;

    if (pp == NULL)
        return;

    freeArray(pp, i);

    free(a);
    a = NULL;
}

/**
 * _realloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @old_size: byte size of previous block
 * @new_size: byte size of new block
 *
 * Return: pointer to da ol'block nameen.
 */
void *allocate_memory(unsigned int size)
{
    char *p = malloc(size);

    return ((!p) ? NULL : p);
}

void copy_memory(void *ptr, char *p, unsigned int size)
{
    do {
        size--;
        p[size] = ((char *)ptr)[size];
    } while (size > 0);
    free(ptr);
}

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *p;

    switch (new_size)
    {
    case 0:
        free(ptr);
        return (NULL);
    default:
        if (!ptr)
            return (allocate_memory(new_size));
			
        if (new_size == old_size)
            return (ptr);

        p = allocate_memory(new_size);
        if (!p)
            return (NULL);

        copy_memory(ptr, p, (old_size < new_size) ? old_size : new_size);
        return (p);
    }
}
