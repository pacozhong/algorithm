#ifndef _H_STACK_H_ 
#define _H_STACK_H_

#include <stdint.h>

typedef struct xl_stack_t 
{
	void *base;//mem pt
	size_t size;//total size of stack
	size_t osize;//object size
	void *cur;//top pt of stack
}
xl_stack_t;

xl_stack_t* xl_stack_init(size_t size, size_t osize);

int xl_stack_peek(xl_stack_t *p, void **item);


int xl_stack_pop(xl_stack_t *p, void **item);


int xl_stack_status(xl_stack_t *p);

size_t xl_stack_count(xl_stack_t *p);


int xl_stack_push(xl_stack_t *p, void *item);

int xl_stack_free(xl_stack_t *p);

#endif

