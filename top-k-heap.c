#include <stdio.h>
#include <stdlib.h>

void swap_data(int *a, int *b){

	int tmp = *a;
	*a = *b;
	*b = tmp;
	
}

//max heap
void heaplify(int heap[], int i, int length){
	
	while(i < length){
		int ichild = 2 * i + 1;
		if(ichild < length - 1 && heap[ichild] < heap[ichild + 1])
		{ichild ++;}
		if(heap[ichild] > heap[i]) {
			//swap
			swap_data(heap + ichild, heap + i);
			i = ichild;
		}else{break;}
	}
}

int
main(int argc, char* argv[])
{
	int n = 0, k = 0;
	scanf("%d, %d", &n, &k);
	
	int *heap = (int *)malloc(sizeof(int) * k);
	int i = 0, tmp = 0, j = 0;

	while(i < n && scanf("%d", &tmp)) {
		if(i < k) *(heap + i) = tmp;
		if(i == k - 1) {
			//create heap
			for(j = (k - 1) / 2; j >= 0; j -- ){
				heaplify(heap, j, k);
			}
		}
		if(i >= k){
			if(tmp < *heap){
				*heap = tmp;
				heaplify(heap, 0, k);
			}
		}
		i ++;
	}
	for(j = 0; j < k; j ++){
		printf("%d\t", *(heap + j));
	}
	printf("\n");
}
