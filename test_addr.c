#include <stdio.h>
#include <stdlib.h>


typedef struct tree_bs_node{
	void * data;
	struct tree_bs_node *l;
	struct tree_bs_node *r;
	size_t cnt;
}tree_bs_node_t;

typedef struct tree_bs
{
	tree_bs_node_t * root;//root node 
	size_t osize;// data size 
	size_t key_offset;//key offset
	size_t key_len;//key len

	size_t num_node;//total node number
} tree_bs_t;

typedef struct st {
	char * a;
	int key;
	char*  pad;
}st;
void change_data(tree_bs_node_t *tmp){
	tmp->data = NULL;
}
int
main(int argc, char *argv[]){
	st* st_1 = (st*)calloc(sizeof(st), 1);
	st_1->a = "aaa";
	st_1->key = 1;
	st_1->pad = "pad";
	tree_bs_node_t *node_1= (tree_bs_node_t*)calloc(sizeof(tree_bs_node_t), 1);
	node_1->data = st_1;
	node_1->l = NULL;
	node_1->r = NULL;
	node_1->cnt = 3;
	
		
	printf("data:0x%016lx\n", (int64_t)(node_1->data));	
	change_data(node_1);
	printf("data:0x%016lx\n", (int64_t)(node_1->data));	

	return 0;
}
