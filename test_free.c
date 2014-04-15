#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char* argv[]){
	if((1 || printf("test\n")) && (0 || printf("xxx\n"))){
		return 1;
	}
	return 0;
}
