#include <stdio.h>
#include <stdlib.h>

void swap_data(int *a, int *b){
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

void bsort(int *a, int length, int *b){
	int i = 0, j = 0;
	for(i = length - 2; i >= 0; i --){
		for(j = 0; j <= i; j ++){
			if(a[j] > a[j + 1]){
				swap_data(a + j, a + j + 1);
				swap_data(b + j, b + j + 1);	
			}
		
		}
	
	}
}

void output(int *a, int length){
	int i = 0;
	for(i = 0; i < length; i ++){
		printf("%d\t", *(a + i));
	}
	printf("\n");
}

int
main(int argc, char *argv[]){

	int length = 0;
	scanf("%d", &length);

	int start = 0, end = 0, k = 0;
	scanf("%d,%d,%d", &start, &end, &k);
	if(end - start + 1 < k){
		printf("wrong input!\n");
		return 0;
	}

	int * a = (int*)malloc(sizeof(int) * length);
	int * b = (int*)malloc(sizeof(int) * length);

	int i =0;
	int tmp = 0;
	while(i < length && scanf("%d", &tmp)){
		*(a + i) = tmp;
		*(b + i) = i;
		i ++;
	}
	output(a, length);
	output(b, length);
	bsort(a, length, b);
	output(a, length);
	output(b, length);
	


	for(i = 0; i <= length - 1; i++){
		if(b[i] >= start && b[i] <= end){
			k--;
			if(k == 0){
				printf("the kth number is:%d\n", a[i]);
				free(a);
				free(b);
				return 0;
			}
		}
	
	}

}
