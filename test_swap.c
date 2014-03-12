#include <stdlib.h>
#include <stdio.h>

void swap_data(int *a, int *b){
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}


int
main(int argc, char* argvv[])
{
	int a = 3;
	int b = 4;
	swap_data(&a, &b);

	printf("%d\t%d\n", a, b);
	return 0;

}
