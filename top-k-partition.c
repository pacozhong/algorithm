#include <stdio.h>
#include <stdlib.h>

void swap_data(int *a, int *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(int data[], int p, int r){
	if(p < 0 || r < 0 || p >= r) return -1;

	int div = data[r];
	int end = r;
	r --;
	while(p < r){
		while(p < r && data[p] <= div) p ++;
		while(p < r && data[r] > div) r --;
		if(p < r) swap_data(data + p, data + r);
	}
	if(data[end] < data[r])
		swap_data(data + end, data + r);
	return r;
}

void get_top_k(int data[], int p, int r, int k){
	if(r - p + 1 <= k) {
		//output
		int j = 0;
		for(j = p; j <= r; j++){printf("%d\t", data[j]);}
		return ;
	}else if(k <= 0) {
		return ;
	}
	int mid  = partition(data, p, r);
	if(-1 == mid) return;
	if(mid - p <= k - 1) {
		get_top_k(data, p, mid, mid - p + 1);
		get_top_k(data, mid + 1, r, k - (mid - p + 1));
	}else {
		get_top_k(data, p, mid - 1, k);
	}
}

int
main(int argc, char* argv[]){
	int n = 0, k = 0;
	scanf("%d,%d", &n, &k);
	
	int *data = (int *)malloc(sizeof(int) * n);

	int i = 0, tmp = 0;
	while(i < n && scanf("%d", &tmp)){
		*(data + i) = tmp;
		i ++;
	}
	get_top_k(data, 0, n - 1, k);
}
