#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int pipe_fds[2];
	pid_t pid;

	if (type != 'r' && type != 'w')
		return -1;

	if (pipe(pipe_fds) == -1)
		return -1;

	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return -1;
	}

	if (pid == 0)
	{
		if (type == 'r')
		{
			close(pipe_fds[0]);  // Close the unused (read) end
			if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(pipe_fds[1]); // Safe to close the write end after dup2
		}
		else if (type == 'w')
		{
			close(pipe_fds[1]);  // Close the unused (write) end
			if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(pipe_fds[0]); // Safe to close the read end after dup2
		}
		execvp(file, argv);
		perror("execvp"); // execvp only returns if there's an error
		exit(EXIT_FAILURE);
	}

	// Parent process
	if (type == 'r')
	{
		close(pipe_fds[1]);	// Close the write end
		return pipe_fds[0]; // Return the reading end
	}
	else if (type == 'w')
	{
		close(pipe_fds[0]); // Close the read end
		return pipe_fds[1]; // Return the writing end
	}
	return -1; // Fallback (will not reach here)
}

int	main(void)
{
	char	buffer[256];
	ssize_t	bytes_read;

	// Test 1: Reading output from "ls"
	int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	if (fd == -1)
		return 1;
	printf("Output of `ls`:\n");
	while ((bytes_read = read(fd, buffer, 256)) > 0)
		write(STDOUT_FILENO, buffer, bytes_read);
	close(fd);

	// Test 2: Writing input to "cat"
	int fd2 = ft_popen("cat", (char *const[]){"cat", NULL}, 'w');
	if (fd2 == -1)
		return 1;
	printf("`cat`:\n");
	while ((bytes_read = read(STDOUT_FILENO, buffer, 256)) > 0)  // until EOF
		write(fd2, buffer, bytes_read);
	close(fd2);

	// Test 3: pipe the output of "ls" to "wc -l" (not very safe, esp valgrind)
	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	if (fd == -1)
		return 1;
	fd2 = ft_popen("wc", (char *const[]){"wc", "-l", NULL}, 'w');
	if (fd2 == -1)
		return 1;
	while ((bytes_read = read(fd, buffer, 256)) > 0)
		write(fd2, buffer, bytes_read);
	close(fd);
	close(fd2);

	return 0;
}
