#include <stdio.h>
#include <stdlib.h>


int __bsearch(int *a, int key, int p, int e){
	if(p > e) return -1;
	if(p == e){
		if(*(a + p) != key) return -1;
		else return p;
	}
	int mid = (p + e) / 2;
	if(*(a + mid) == key) return mid;
	else if(*(a + mid) > key) return __bsearch(a, key, p, mid - 1);
	else return __bsearch(a, key, mid + 1, e);
}

void swap_data(int *a , int *b){
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

void isort(int *a, int length){
	int i = 0;
	int j = 0;
	int k = 0;
	int tmp = 0;
	for(i = 0; i < length - 1; i ++){
		k = i + 1;
		if(*(a + k) < *(a + i)){
			//save data to be insert
			tmp = *(a + k);
			for(j = i; j >= 0; j --){
				if(tmp < *(a + j)){
					//move data
					*(a + j + 1) = *(a + j);
				}else break;
			}
			//insert
			*(a + j + 1) = tmp;
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

int _bsearch(int *a, int length, int key){
	return __bsearch(a, key, 0, length - 1);
}

int
main(int argc, char* argv[]){

	int length = 0;
	scanf("%d", &length);
	int *a = (int *)malloc(sizeof(int) * length);
	int i = 0;
	while(i < length){
		scanf("%d", a + i);
		i ++;
	}

	output(a, length);
	isort(a, length);
	output(a, length);
	int key = 0;
	int index = -1;
	while(scanf("%d", &key) != EOF){
		index = _bsearch(a, length, key);
		if(-1 == index)printf("can not find\n");
		else printf("index:%d\n", index);
	}
}
