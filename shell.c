#include "main.h"
/**
  * main - entry point of the program
  * @c: counter
  * @v: vector
  * @arrp: array of strings.
  * Return: 0
  */
int main(int c __attribute__((unused)), char *v[], char *arrp[])
{
	char *buffer = NULL;
	size_t bufsize = 0;
	int status;
	pid_t child_pid;

	signal(SIGINT, SIG_IGN);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("~$ ");
		if (getline(&buffer, &bufsize, stdin) == -1)
			break;
		if (buffer == NULL)
			exit(0);
		v = parse_input_string(buffer);
		if (!v[0])
		{
			free(v);
			continue;
		}
		if (_strcmp(v[0], "env") == 0)
		{
			print_environ(), free(v);
			continue;
		}
		if (_strcmp(v[0], "exit") == 0)
			free(v), free(buffer), exit(0);
		child_pid = fork();
		if (child_pid == 0)
		{
			if (_strchr(v[0], '/') == NULL)
				v[0] = path_search(v[0]);
			if (execve(v[0], v, arrp))
			{
				perror("execve"), exit(EXIT_FAILURE);
				break;
			}
		}
		wait(&status), free(v);
	}
	free(buffer);
	return (0);
}
