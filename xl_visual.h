#ifndef _H_XL_VISUAL_H_ 
#define _H_XL_VISUAL_H_

#include "xl_bst.h"
#include "xl_rbt.h"
#include "xl_stack.h"

typedef int xl_get_key_fun_t(void *data);	

int xl_visual_bst(xl_bst_t *bst, char* filename, xl_get_key_fun_t *fun);

#endif

