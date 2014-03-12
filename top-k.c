#include <stdio.h>
#include <stdlib.h>

int get_max(int *p, int k, int * index){
	int tmp = *p;
	*index = 0;
	int i = 0;
	while(i < k){
		if(*(p + i) > tmp) {
			tmp = *(p + i);
			*index = i;
		}
		i ++;
	}
	return tmp;
}

int
main(int argc, char *argv[])
{
	int n = 0, k = 0;
	scanf("%d,%d", &n, &k);
	if(n < k || k <=0 ) {
		printf("error input\n");
		return 0;
	}
	printf("n:%d\tk:%d\n", n, k);
	int * pk = (int*)malloc(k * sizeof(int));
	int * pnk = (int*)malloc((n - k) * sizeof(int));
	int tmp = 0;
	int i = 0;
	while(i < n && scanf("%d", &tmp)) {
		if(i < k) *(pk + i) = tmp;
		else *(pnk + i - k) = tmp;
		i ++;
	}
	for(i = 0; i < n; i ++)
	{
		if(i < k) printf("%d\t", *(pk + i));
		else printf("%d\t", *(pnk + i - k));
	}
	printf("\n");
	int j = 0;
	while(j < n - k) {
		int tmp_index = 0;
		int tmp_max = get_max(pk, k, &tmp_index);
		for(j = 0; j < n - k; j ++){
			if(*(pnk + j) < tmp_max ){
				*(pk + tmp_index) = *(pnk + j);
				*(pnk + j) = tmp_max;
				break;
			}
		}	
	}

	for(i = 0; i < n; i ++)
	{
		if(i < k) printf("%d\t", *(pk + i));
		else printf("%d\t", *(pnk + i - k));
	}
	printf("\n");
	free(pk);
	free(pnk);
}
