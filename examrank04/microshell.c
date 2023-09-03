#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int	_kill(char *reason)
{
	while (*reason)
		write(2, reason++, 1);
	return (1);
}

int	_cd(char **argv, int i)
{
	if (i != 2)
		return (_kill("error: cd: bad arguments\n"));
	else if (chdir(argv[1]) == -1)
		return (_kill("error: cd: cannot change directory to "), _kill(argv[1]), _kill("\n"));
	return (0);
}

int	_execute(char **argv, char **env, int i)
{
	int prcs;
	int piped;
	int fd[2];
	int pid;

	piped = argv[i] && !strcmp(argv[i], "|");
	if (piped && pipe(fd) == -1)
		return (_kill("error: fatal\n"));
	pid = fork();
	if (!pid)
	{
		argv[i] = 0;
		if (piped && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return (_kill("error: fatal\n"));
		execve(*argv, argv, env);
		return (_kill("error cannot execute "), _kill(*argv), _kill("\n"));
	}
	waitpid(pid, &prcs, 0);
	if (piped && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		return (_kill("error: fatal\n"));
	return(WIFEXITED(prcs) && WEXITSTATUS(prcs));
}

int	main(int argc, char **argv, char **envp)
{
	int i;
	int preset;

	i = 0;
	preset = 0;
	if (argc > 1)
	{
		while (argv[i] && argv[++i])
		{
			argv += i;
			i = 0;
			while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
				i++;
			if (!strcmp(*argv, "cd"))
				preset = _cd(argv, i);
			else if (i)
				preset = _execute(argv, envp, i);
		}
	}
	return (preset);
}

