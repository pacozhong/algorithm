#ifndef _H_TREE_BS_H_ 
#define _H_TREE_BS_H_

typedef struct xl_bst_node{
	void * data;
	struct xl_bst_node *l;
	struct xl_bst_node *r;
	size_t cnt;
}xl_bst_node_t;

/* 
 * function to compare two node,
 * return -1/1/0 whern a little/greater/equal to b
 */
typedef int xl_bst_compare_fun_t(const void *a, const const void *b);

typedef void xl_bst_visit_fun_t(const xl_bst_node_t *node);

typedef struct xl_bst
{
	xl_bst_node_t * root;//root node 

	xl_bst_compare_fun_t *compare_fun;
	size_t num_node;//total node number
} xl_bst_t;

xl_bst_t *xl_bst_init(xl_bst_compare_fun_t* compare_fun);

int xl_bst_free(xl_bst_t *tr);

int xl_bst_destroy(xl_bst_t *tr);

int xl_bst_find(xl_bst_t *tree, const void* data, xl_bst_node_t **ret_node);

int xl_bst_insert(xl_bst_t *tree, void* data);

int xl_bst_delete(xl_bst_t *tree, const void *data);

int xl_bst_left_rotate(xl_bst_t *tree, const void *data);

int xl_bst_right_rotate(xl_bst_t *tree, const void *data);

void xl_bst_inorder_traverse(xl_bst_t *tree, xl_bst_visit_fun_t *visit);

void xl_bst_preorder_traverse(xl_bst_t *tree, xl_bst_visit_fun_t *visit);

void xl_bst_postorder_traverse(xl_bst_t *tree, xl_bst_visit_fun_t *visit);
#endif

