#ifndef _H_TREE_RB_ 
#define _H_TREE_RB_

#define XL_RED 1
#define XL_BLACK 2
typedef struct xl_rbt_node {
	void *data;
	struct xl_rbt_node *l;
	struct xl_rbt_node *r;
	size_t cnt;
	
	int color;	
	struct xl_rbt_node *p;
} xl_rbt_node_t;

/* 
 * function to compare two node,
 * return -1/1/0 whern a little/greater/equal to b
 */
typedef int xl_rbt_compare_fun_t(const void *a, const void *b);

typedef void xl_rbt_visit_fun_t(const xl_rbt_node_t *node, void *data);

typedef struct xl_rbt{
	xl_rbt_node_t *root;
	
	xl_rbt_compare_fun_t *compare_fun;
	size_t num_node;	
} xl_rbt_t;


xl_rbt_t* xl_rbt_init(xl_rbt_compare_fun_t *comapre_fun);

int xl_rbt_free(xl_rbt_t *tree);

int xl_rbt_destroy(xl_rbt_t *tree);

int xl_rbt_find(xl_rbt_t *tree, const void *data, xl_rbt_node_t **ret_node);


int xl_rbt_insert(xl_rbt_t *tree, void *data);


int xl_rbt_delete(xl_rbt_t *tree, const void *data);


void xl_rbt_inorder_traverse(xl_rbt_t *tree, xl_rbt_visit_fun_t *visit, void *data);


void xl_rbt_preorder_traverse(xl_rbt_t *tree, xl_rbt_visit_fun_t *visit, void *data);


void xl_rbt_postorder_traverse(xl_rbt_t *tree, xl_rbt_visit_fun_t *visit, void *data);

#endif

