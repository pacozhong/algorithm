#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "xl_stack.h"

#define NO_ELE -1;//no element in stack
#define BAD_POS -2;//disorder cur position 

/* 
 * init a stack
 * size: stack size
 * osize: the object size
 */
xl_stack_t* xl_stack_init(size_t size, size_t osize){
	xl_stack_t *p = (xl_stack_t*)calloc(sizeof(xl_stack_t), 1);
	p->base = calloc(osize, size);
	p->osize = osize;
	p->size = size;
	p->cur = p->base;
	return p;
}

int xl_stack_peek(xl_stack_t *p, void **item){
	if(p->base == p->cur){return NO_ELE;}
	
	if(p->cur - p->osize >= p->base) {
		*item = p->cur - p->osize;
		return 0;
	}
	else return BAD_POS;
}

int xl_stack_pop(xl_stack_t *p, void **item){	
	if(p->base == p->cur){return NO_ELE;}
	
	if(p->cur - p->osize >= p->base) {
		p->cur = p->cur - p->osize;
		if(item != NULL)
			*item = p->cur;
		return 0;
	}else return BAD_POS;
}


size_t xl_stack_count(xl_stack_t *p){
	return (p->cur - p->base) / p->osize;
}

int xl_stack_push(xl_stack_t *p, void *item){
	//check whether mem is enough	
	size_t count = xl_stack_count(p);
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


int xl_stack_status(xl_stack_t *p){
	int8_t base[] = "0123456789ABCDEF";
	void *pt = p->cur - p->osize;
	printf("oszie:%lu, size:%lu, count:%lu\n", p->osize, p->size, xl_stack_count(p));
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

int xl_stack_free(xl_stack_t *p){
	if(p->base)
		free(p->base);
	p->base = NULL;
	if(p){
		free(p);
	}
	return 0;
}
