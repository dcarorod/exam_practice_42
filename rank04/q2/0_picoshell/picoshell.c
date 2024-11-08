#include <stdio.h>

#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	int	cmds_size = 0;
	int	i = -1;

	while (cmds[++i])
		cmds_size++;

	pid_t	pid[cmds_size], w[cmds_size];
	int	fds[cmds_size][2];
	i = -1;
	if (pipe(fds[0]) == -1)
		return 1;
	while (++i < cmds_size)
	{
		if (i == 0) // first child
		{
			if (close(fds[0] == -1))
				return 1;
			if (dup2(fds[i][1], STDOUT_FILENO) == -1)
				return 1;
		}
		if (i == cmds_size - 1) // last child
		{
			if (close(fds[1] == -1))
				return 1;
			if (dup2(fds[0], STDIN_FILENO) == -1)
				return 1;
		}
		else if (i > 0) // mid children, reuses pipe
		{
			if (close(fds[i - 1][0]) == -1)
				return 1;
			if (pipe(fds) == -1)
				return 1;
			if (dup2(fds[0], STDIN_FILENO) == -1)
				return 1;
		}
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			return 1;
		pid[i] = fork();
		if (pid[i] < 0)
			return 1;
		if (pid[i] == 0)
		{
			;
		}
		else
			wait(&w[i]);
	}

	return 0;
}
