#include <stdio.h>
#include <stdlib.h>

int print_star(int n){
	if(n  % 2 == 0) return -1; 
	int x = 0, y = 0, i = 0;
	for(i = 0; i <= n * n - 1; i ++){
		x = i % n;
		y = i / n;
		if(x + y - n / 2 >= 0 &&
			x + y - (n / 2)* 3 <= 0 &&
			y - x - n / 2 <= 0 && 
			x - y - n / 2 <= 0)
			printf("*");
		else printf(" ");
		if((i + 1) % n == 0) printf("\n");
	}
	return 0;
}

int
main(int argc, char* argv[]){

	int n = 0;
	while(scanf("%d", &n) != EOF){
		if(0 != print_star(n)) printf("error input!\n");
	}
	return 0;
}
