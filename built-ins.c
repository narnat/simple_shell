#include "simple_shell.h"

char *_getenv2(char *name, arguments_t *env)
{
	int i = 0, len = _strlen(name);
	(void) env;

	while((environ)[i])
	{
		if (!_strncmp(name, (environ)[i], len))
		{
			name = (environ)[i] + len + 1;
			return (name);
		}
		i++;
	}
	return (NULL);
}


/**
 * print_env - Prints environmental variables
 */
int print_env(arguments_t *args)
{
	size_t i = 0;

	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		_puts("\n");
		++i;
	}
	free(args->arr);
	return (1);
}

int call_exit(arguments_t *args)
{
	free(args->arr);
	free(args->buf);
	//free(args->_environ);
	free_list(args->head);
	exit(0);
}

int custom_cd(arguments_t *args __attribute__((unused)))
{
	char *home = _getenv2("HOME", args);
	char *cwd = getcwd(cwd = NULL, 0);
	char *oldwd = _getenv2("OLDPWD", args);

	if (!args->arr[1] || *args->arr[1] == '=' || !_strcmp(args->arr[1], "$HOME"))
	{
		chdir(home);
	}

	if (*args->arr[1] == '-')
	{
		chdir(oldwd);
	}
	chdir(args->arr[1]);
	free(cwd);
	free(args->arr);
	return (1);
}

int builtins(arguments_t *args)
{
	built_ins_t func_array[] = {
		{"env", print_env},
		{"exit", call_exit},
		{"cd", custom_cd},
		{"unsetenv", _unsetenv},
		{"setenv", _setenv},
		{NULL, NULL}
	};
	int  i = 0;

	while (func_array[i].bi)
	{
		if (!_strcmp(args->arr[0], func_array[i].bi))
		{
			func_array[i].f(args);
			return (1);
		}
		i++;
	}
	return (0);
}
