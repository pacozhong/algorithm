#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stack.h"

#define NO_ELE -1;//no element in stack
#define BAD_POS -2;//disorder cur position 

/* 
 * init a stack
 * size: stack size
 * osize: the object size
 */
stack_t* stack_init(size_t size, size_t osize){
	stack_t *p = (stack_t*)calloc(sizeof(stack_t), 1);
	p->base = calloc(osize, size);
	p->osize = osize;
	p->size = size;
	p->cur = p->base;
	return p;
}

int stack_peek(stack_t *p, void **item){
	if(p->base == p->cur){return NO_ELE;}
	
	if(p->cur - p->osize >= p->base) {
		*item = p->cur - p->osize;
		return 0;
	}
	else return BAD_POS;
}

int stack_pop(stack_t *p, void **item){	
	if(p->base == p->cur){return NO_ELE;}
	
	if(p->cur - p->osize >= p->base) {
		p->cur = p->cur - p->osize;
		if(item != NULL)
			*item = p->cur;
		return 0;
	}else return BAD_POS;
}


size_t stack_count(stack_t *p){
	return (p->cur - p->base) / p->osize;
}

int stack_push(stack_t *p, void *item){
	//check whether mem is enough	
	size_t count = stack_count(p);
	if( count >= p->size){
		//expand base
		p->base = realloc(p->base, p->size * 2 * p->osize);
	    p->size = p->size * 2;
		p->cur = p->base + count * p->osize;
	}
	memcpy(p->cur, item, p->osize);
	p->cur += p->osize;
	return 0;
}


int stack_status(stack_t *p){
	int8_t base[] = "0123456789ABCDEF";
	void *pt = p->cur - p->osize;
	printf("oszie:%lu, size:%lu, count:%lu\n", p->osize, p->size, stack_count(p));
	while(pt >= p->base){
		void *tmp = pt;
		printf("0x%016lx\t", (int64_t)tmp);

		while(tmp < pt + p->osize){
			printf("0x%c%c, ", base[(*((int8_t*)tmp) >> 4) & 0x0F], base[*((int8_t*)tmp) & 0x0F]);
			tmp ++;
		}		
		printf("\n");
		pt = pt - p->osize;
	}
	return 0;
}

int stack_free(stack_t *p){
	if(p->base)
		free(p->base);
	p->base = NULL;
	if(p){
		free(p);
	}
	return 0;
}
/* 
int
main(int argc, char *argv[]){
	stack_t *p = stack_init(10, sizeof(int));
	int tmp = 0;
	int i = 3;
	while(i -- > 0){
		while(scanf("%d", &tmp) != EOF){
			printf("pbase:%lx\n", (int64_t)p->base);
			stack_push(p, (void *) &tmp);
			printf("pbase:%lx\n", (int64_t)p->base);
			stack_status(p);
		}
		int * ret = NULL;
		int retInt = 0;
		while(printf("1-peek, 2-pop:") && (scanf("%d", &tmp) != EOF)){
			if(tmp == 1) {
				retInt = stack_peek(p, (void **) &ret);
				if(retInt != 0){printf("peek error!\n");}
				else {printf("peek data:%d\n", *ret);}
			}else if(tmp == 2){
				retInt = stack_pop(p, (void **) &ret);
				if(retInt != 0){printf("pop error!\n");}
				else {printf("pop data:%d\n", *ret);}
			}
			stack_status(p);
		}	
	}
	stack_free(p);
	return 0;
}
*/
