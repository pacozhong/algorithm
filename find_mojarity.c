#include <stdio.h>
#include <stdlib.h>

int find_majority(int *a, int length){
	int tmp = 0;
	int cnt = 0;
	int i = 0;
	while(i < length){
		if(cnt == 0) {
			tmp = *(a + i);
			cnt ++;
		}else if(tmp == *(a + i)){
			cnt ++;
		}else if(tmp != *(a + i)){
			cnt --;
		}
		i ++;
	}
	return tmp;
}


int
main(int argc, char *argv[]){
	int length = 0;
	scanf("%d", &length);
	int *a = (int *)malloc(sizeof(int) * length);
	int i = 0;
	while(i < length && scanf("%d", a + i) != EOF) i ++;
	printf("%d\n", find_majority(a, length));
	return 0;
}
