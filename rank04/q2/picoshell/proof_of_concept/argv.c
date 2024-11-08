#include <stdio.h>

int	main(int argc, char **argv)
{
	int	i = 0;
	while(argv[++i])
		printf("argv[%d]: %s\n", i, argv[i]);
	return 0;
}
