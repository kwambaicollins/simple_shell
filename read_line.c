#include "holberton.h"

/**
 * read_line - reads the input string.
 *
 * @i_eof: returns value of getline function
 * Return: inputs string
 */
char *read_line(int *i_eof)
{
	char *input = NULL;
	size_t bufsize = 0;

	*i_eof = getline(&input, &bufsize, stdin);

	return (input);
}
