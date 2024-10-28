#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int	main(void)
{
	printf("INFINITY - INFINITY: %f\n", INFINITY - INFINITY);
	printf("INFINITY - INFINITY + 1: %f\n", INFINITY - INFINITY + 1);
	printf("INFINITY + 4: %f\n", INFINITY + 4);
	printf("INFINITY == INFINITY: %s\n", INFINITY == INFINITY ? "true" : "false");
	printf("INFINITY + 1 == INFINITY + 2: %s\n", INFINITY + 1 == INFINITY + 2 ? "true" : "false");
}
