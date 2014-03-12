#ifndef _H_STACK_H_ 
#define _H_STACK_H_

#include <stdint.h>

typedef struct stack_t 
{
	void *base;//mem pt
	size_t size;//total size of stack
	size_t osize;//object size
	void *cur;//top pt of stack
}
stack_t;

stack_t* stack_init(size_t size, size_t osize);

int stack_peek(stack_t *p, void **item);


int stack_pop(stack_t *p, void **item);


int stack_status(stack_t *p);

size_t stack_count(stack_t *p);


int stack_push(stack_t *p, void *item);

int stack_free(stack_t *p);

#endif

