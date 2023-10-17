#include "holberton.h"

/**
 * is_cdir - checks ":" if is in the current directory.
 * @path: type char pointer char.
 * @i: type int pointer of index.
 * Return: 1 if the path is searchable in the cd, 0 otherwise.
 */
int is_cdir(char *path, int *i)
{
	if (path[*i] == ':')
		return (1);

	while (path[*i] != ':' && path[*i])
	{
		*i += 1;
	}

	if (path[*i])
		*i += 1;

	return (0);
}

/**
 * _which - locates a command
 *
 * @cmd:it is the command name
 * @_environ:gives environment variable
 * Return: location of the command.
 */
char *_which(char *cmd, char **_environ)
{
	char *path, *ptr_path, *token_path, *dir;
	int len_dir, len_cmd, i;
	struct stat st;

	path = _getenv("PATH", _environ);
	if (path)
	{
		ptr_path = _strdup(path);
		len_cmd = _strlen(cmd);
		token_path = _strtok(ptr_path, ":");
		i = 0;
		while (token_path != NULL)
		{
			if (is_cdir(path, &i))
				if (stat(cmd, &st) == 0)
					return (cmd);
			len_dir = _strlen(token_path);
			dir = malloc(len_dir + len_cmd + 2);
			_strcpy(dir, token_path);
OBOBOBOBOBOBOBOBOBOBOBOB			_strcat(dir, "/");
OBOBOBOBOBOBOBOBOB			_strcat(dir, cmd);
OBOBOBOBOB			_strcat(dir, "\0");
			if (stat(dir, &st) == 0)
			{
				free(ptr_path);
				return (dir);
OBOBOBOBOBOB			}
			free(dir);
OBOBOBOBOBOB			token_path = _strtok(NULL, ":");
OBOBOBOBOBOB		}
		free(ptr_path);
OBOBOBOB		if (stat(cmd, &st) == 0)
OBOBOBOBOB			return (cmd);
OBOBOBOB		return (NULL);
OBOBOBOB	}
OBOBOB	if (cmd[0] == '/')
OBOBOB		if (stat(cmd, &st) == 0)
OBOBOB			return (cmd);
OBOBOB	return (NULL);
}

/**
 * is_executable - determines if is an executable
OBOBOBOBOBOBOBOBOBOBOB *
 * @datash: data structure
OBOBOBOBOBOBOBOBOBOBOBOBOBOBOB * Return: 0 if is not an executable, other number if it does
OBOBOBOBOBOBOBOBOB */
int is_executable(data_shell *datash)
OBOBOBOBOBOBOBOBOB{
OBOBOBOBOBOBOBOBOBOBOBOBOBOBOBOB	struct stat st;
OBOBOBOBOBOB	int i;
OBOBOBOBOBOB	char *input;
OBOBOBOBOBOB
OBOBOBOBOBOBOBOBOBOBOBOBOBOB	input = datash->args[0];
OBOBOBOBOBOBOB	for (i = 0; input[i]; i++)
OBOBOB	{
		if (input[i] == '.')
OBOBOBOBOBOB		{
OBOBOBOBOBOBOBOB			if (input[i + 1] == '.')
OBOBOBOBOBOB				return (0);
OBOBOB			if (input[i + 1] == '/')
				continue;
OBOB			else
				break;
OBOBOB		}
		else if (input[i] == '/' && i != 0)
OBOB		{
OB			if (input[i + 1] == '.')
OBOB				continue;
			i++;
OB			break;
		}
OBOB		else
OB			break;
OB	}
OBOB	if (i == 0)
		return (0);
OB
	if (stat(input + i, &st) == 0)
	{
OB		return (i);
	}
OB	get_error(datash, 127);
	return (-1);
}

OB/**
 * check_error_cmd - verifies if user has permissions to access
 *
OBOB * @dir: destination directory
 * @datash: data structure
 * Return: 1 if there is an error, 0 if not
 */
OBint check_error_cmd(char *dir, data_shell *datash)
{
	if (dir == NULL)
	{
		get_error(datash, 127);
		return (1);
OB	}

	if (_strcmp(datash->args[0], dir) != 0)
OB	{
		if (access(dir, X_OK) == -1)
		{
			get_error(datash, 126);
			free(dir);
			return (1);
		}
		free(dir);
	}
	else
	{
		if (access(datash->args[0], X_OK) == -1)
		{
			get_error(datash, 126);
			return (1);
		}
	}

	return (0);
}

/**
 * cmd_exec - executes command lines
 *
 * @datash: data relevant (args and input)
 * Return: 1 on success.
 */
int cmd_exec(data_shell *datash)
{
	pid_t pd;
	pid_t wpd;
	int state;
	int exec;
	char *dir;
	(void) wpd;

	exec = is_executable(datash);
	if (exec == -1)
		return (1);
	if (exec == 0)
	{
		dir = _which(datash->args[0], datash->_environ);
		if (check_error_cmd(dir, datash) == 1)
			return (1);
	}

	pd = fork();
	if (pd == 0)
	{
		if (exec == 0)
			dir = _which(datash->args[0], datash->_environ);
		else
			dir = datash->args[0];
		execve(dir + exec, datash->args, datash->_environ);
	}
	else if (pd < 0)
	{
		perror(datash->av[0]);
		return (1);
	}
	else
	{
		do {
			wpd = waitpid(pd, &state, WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
	}

	datash->status = state / 256;
	return (1);
}
