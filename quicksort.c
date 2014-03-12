
#include <stdlib.h>
#include <stdio.h>

void swap_data(int *a, int *b){
	*a = (*a) ^ (*b);
	*b = (*a) ^ (*b);
	*a = (*a) ^ (*b);
/*  	
	int tmp = *a;
	*a = *b;
	*b = tmp;
	*/
}

int partition(int data[], int p, int r)
{
	//choose the last item as the point of division
	if(p < 0 || r < 0 || p >= r) return -1;
	int div = data[r];
	int end = r;
	r --;
	while(p < r){
		while(p < r && data[p] <= div) p ++;
		while(p < r && data[r] > div) r --;
		if(p < r)
			swap_data(data + p, data + r);
	}
	if(data[p] > data[end])
		swap_data(data + end, data + p);
	return r;
}

void quick_sort(int data[], int p, int r)
{
	int middle = partition(data, p, r);
	if(-1 != middle){
		quick_sort(data, p, middle-1);
		quick_sort(data, middle + 1, r);
	}
}

int
main(int argc, char *argv[])
{
	int n = 0;
	scanf("%d", &n);
	int * data = (int *)malloc(sizeof(int) * n);
	int i = 0, tmp = 0;
	while(i < n && scanf("%d", &tmp)){
		*(data + i) = tmp;
		i ++;
	}
	for(i = 0; i < n; i ++) {printf("%d\t", data[i]);}
	printf("\n");
	quick_sort(data, 0, n - 1);

	for(i = 0; i < n; i ++) {printf("%d\t", data[i]);}
	printf("\n");
	return 0;
}
