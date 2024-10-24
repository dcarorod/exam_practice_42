#include <stdio.h>
#include <stdlib.h>

void	process(int target, int aggr_solution, int *nums, int *candidates, int idx, int size)
{
	if (idx == size)
	{
		if

int	main(int argc, char **argv)
{
	if (argc < 3)
		return (1);
	int	target = atoi(argv[1]);

	int	size = argc - 2;
	int	nums[size];
	int	i = -1;
	while (++i < size)
		nums[i] = atoi(argv[i + 2]);
	i = -1;
	while (++i < size)
		printf("nums[%d]: %d\n", i, nums[i]);
}
