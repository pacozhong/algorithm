#ifndef _H_TREE_BS_H_ 
#define _H_TREE_BS_H_

typedef struct tree_bs_node{
	void * data;
	struct tree_bs_node *l;
	struct tree_bs_node *r;
	size_t cnt;
}tree_bs_node_t;

/* 
 * function to compare two node,
 * return -1/1/0 whern a little/greater/equal to b
 */
typedef int tree_bs_compare_fun_t(const void *a, const const void *b);

typedef void tree_bs_visit_fun_t(const tree_bs_node_t *node);

typedef struct tree_bs
{
	tree_bs_node_t * root;//root node 

	tree_bs_compare_fun_t *compare_fun;
	size_t num_node;//total node number
} tree_bs_t;

tree_bs_t *tree_bs_init(tree_bs_compare_fun_t* compare_fun);

int tree_bs_free(tree_bs_t *tr);

int tree_bs_destroy(tree_bs_t *tr);

int tree_bs_find(tree_bs_t *tree, const void* data, tree_bs_node_t **ret_node);

int tree_bs_insert(tree_bs_t *tree, void* data);

int tree_bs_delete(tree_bs_t *tree, const void *data);

int tree_bs_left_rotate(tree_bs_t *tree, const void *data);

int tree_bs_right_rotate(tree_bs_t *tree, const void *data);

void tree_bs_inorder_traverse(tree_bs_t *tree, tree_bs_visit_fun_t *visit);

void tree_bs_preorder_traverse(tree_bs_t *tree, tree_bs_visit_fun_t *visit);

void tree_bs_postorder_traverse(tree_bs_t *tree, tree_bs_visit_fun_t *visit);
#endif

