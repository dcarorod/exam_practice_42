#include <stdio.h>

int	main(void)
{
	char	*immutable = "my immutable string";
	char	mutable[20] = "my mutable string"; // 17 needed, gave 20
	char	versatile[] = "also a mutable string but no letter counting";

	// immutable[0] = 'X';  // segfaults
	mutable[0] = 'X';
	versatile[0] = 'X';

	puts(immutable);
	puts(mutable);
	puts(versatile);

	// verify pos of idx of '\0'
	int i = -1;
	while (mutable[++i]);
		printf("%c", mutable[i]);
	printf("\n");
	printf("idx of 0 terminator for mutable[20] with strlen of 17 is: %d\n", i);
}
