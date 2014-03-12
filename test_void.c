#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char* argvp[])
{
	int *p = calloc(sizeof(int), 8);
	*p = 1;
	*(p + 1) = 2;
	
	printf("%d\t%d\n", *p, *(p + 1));

	char *pi = (char*)p;

	printf("%d\t%d\n", *pi, *(pi + 1));

//	printf("%d\t%d\n", sizeof(char), sizeof(int));


	printf("%lu\t", sizeof(int8_t*));
	return 0;
}
