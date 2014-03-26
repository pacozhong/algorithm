#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define RED 1
#define BLACK 2

typedef struct tree_rb_node {
	void *data;
	struct tree_rb_node *l;
	struct tree_rb_node *r;
	int cnt;
	
	int color;	
	struct tree_rb_node *p;
} tree_rb_node_t;

/* 
 * function to compare two node,
 * return -1/1/0 whern a little/greater/equal to b
 */
typedef int tree_rb_compare_fun_t(const void *a, const const void *b);

typedef void tree_rb_visit_fun_t(const tree_rb_node_t *node);

typedef struct tree_rb{
	tree_rb_node_t *root;
	
	tree_rb_compare_fun_t *compare_fun;
	size_t num_node;	
} tree_rb_t;


tree_rb_t* tree_rb_init(tree_rb_compare_fun_t *comapre_fun){
	tree_rb_t * tr = (tree*)calloc(sizeof(tree_rb_t), 1);
	if(tr == NULL) return NULL;
	tr->root = NULL;
	tr->compare_fun = compare_fun;
	tr->num_node = 0;
	return tr;
}

static int _tree_rb_find(tree_rb_t* tree, tree_rb_node_t *node, void const *data, 
		tree_rb_node_t *p, tree_rb_node_t **ret_node){
	if(tree == NULL) return -2;	
	if(node == NULL) {
		if(ret_node != NULL)
			*ret_node = p;
		return -1;
	}
	int compare_ret = tree->compare_fun(data, node->data);
	if(0 == compare_ret){
		if(ret_node != NULL)
			*ret_node = node;
		return 0;
	}else if(comapre_ret > 0){
		return _tree_rb_find(tree, node->r, data, node, ret_node);
	}else {
		return _tree_rb_find(tree, node->l, data, node, ret_node);
	}
}

int tree_rb_find(tree_rb_t *tree, void const *data, tree_rb_node_t **ret_node){
	return _tree_rb_find(tree, tree->root, data, NULL, ret_node);
}

/* left rotation */
static int _tree_rb_left_rotate(tree_rb_node_t *pivot){
	if(pivot == NULL || pivot->r == NULL) return -1;
	tree_rb_node_t *tmp = pivot->r;
	pivot->r = tmp->l;
	tmp->l = pivot;
	if(pivot->p != NULL){
		if(pivot->p->l == pivot) pivot->p->l = tmp;
		else pivot->p->r = tmp;
	}
	tmp->p = pivot->p;
	pivot->p = tmp;
	return 0;	
}

/* right rotate */
static int _tree_rb_right_rotate(tree_rb_node_t *pivot){
	if(pivot == NULL || pivot->l == NULL ) return -1;
	tree_rb_node_t *tmp = pivot->l;
	pivot->l = tmp->r;
	tmp->r = pivot;
	if(pivot->p != NULL){
		if(pivot->p->l == pivot) pivot->p->l = tmp;
		else pivot->p->r = tmp;
	}
	tmp->p = pivot->p;
	pivot->p = tmp;
	return 0;
}

static void _tree_rb_insert_fix(tree_rb_t* tree, tree_rb_node_t *new_node){
	while(new_node->p->color = RED){
		if(new_node->p == new_node->p->p->l){
			if(new_node->p->p->r->color == RED){
				new_node->p->p->r->color = BLACK;
				new_node->p->color = BLACK;
				new_node->p->p->color = RED;
				new_node = new_node->p->p;
			}else {
				if(new_node->p->r == new_node){
					new_node = new_node->p;
					//do not need to ajust root, cause pivot has parent
					_tree_rb_left_rotate(new_node);
				}
				else {
					new_node->p->color = BLACK;
					new_node->p->p->color = RED; 	
					_tree_rb_right_rotate(new_node->p->p);	
					//ajust root
					if(tree->root == new_node->p->r){
						tree->root = new_node->p;
					}
				}
			
			}
		}else {
			if(new_node->p->p->l->color == RED){
				new_node->p->p->l->color = BLACK;
				new_node->p->color = BLACK;
				new_node->p->p->color = RED;
				new_node = new_node->p->p;
			}else {
				if(new_node->p->l == new_node){
					new_node = new_node->p;
					_tree_rb_right_rotate(new_node);
				}else {
					new_node->p->color = BLACK;
					new_node->p->p->color = RED;
					_tree_rb_left_rotate(new_node->p->p);
					if(tree->root == new_node->p->l){
						tree->root = new_node->p;
					}
				}
			
			}
		}
	
	}	
}

int tree_rb_insert(tree_rb_t *tree, void const *data){
	if(tree == NULL) return -2;
	tree_rb_node_t *ret_node;
	int find_ret = _tree_rb_find(tree, tree->root, data, NULL, &ret_node);
	if(0 == find_ret){
		ret_node->cnt ++;
		return 0;
	}
	//find fail, add node
	tree->num_node ++;
	tree_rb_node_t *new_node = (tree_rb_node_t*)calloc(sizeof(tree_rb_node_t), 1);
	new_node->l = NULL;
    new_node->r = NULL;
	new_node->color = RED;	
	new_node->data = data;
	new_node->p = NULL;
	new_node->cnt = 1;
	//root is NULL
	if(ret_node == NULL){
		tree->root = new_node;
		new_node->color = BLACK;
		return 0;	
	}
	//root is not NULL, insert
	new_node->p = ret_node;
	int compare_ret = tree->compare_fun(data, ret_node->data);
	if(compare_ret > 0){
		ret_node->r = new_node;
	}else {
		ret_node->l = new_node;
	}
	//fix
	_tree_rb_insert_fix(tree, new_node);
	return 0;
}

//delete fix
void _tree_rb_delete_fix(tree_rb_t *tree, tree_rb_node_t *x){
	tree_rb_node_t *w;
	while(x->color == BLACK && x != tree->root){
		if(x == x->p->l){
			w = x->p->r;
			if(w->color == RED){
				w->color = BLACK;
				x->p->color = RED;
				_tree_rb_left_rotate(x->p);
				//ajust root
				if(x->p == tree->root){
					tree->root = x->p->p;
				}
				w = x->p->r;
			}
			if(w->l->color == BLACK && w->r->color == BLACK){
				w->color = RED;
				x = x->p;
			}else if(w->r->color == BLACK){
				//w->l->color == RED
				w->l->color = BLACK;
				w->color = RED;
				_tree_rb_right_rotate(w);
				w = x->p->r;
			}else{
				//w->r->color == RED, w->l->color RED OR BLACK
				w->color = x->p->color;
				x->p->color = BLACK;
				w->r->color = BLACK;
				_tree_rb_left_rotate(x->p);
				//ajust root
				if(x->p == tree->root) tree->root = x->p->p;
				x = x->p->p;
			}
		}else {
			w = x->p->l;
			if(w->color == RED){
				w->color = BLACK;
				x->p->color = RED;
				_tree_rb_right_rotate(x->p);
				//ajust root
				if(x->p == tree->root) tree->root = x->p->p;
				w = x->p->l;
			}
			if(w->l->color == BLACK && w->r->color == BLACK){
				w->color = RED;
				x = x->p;
			}else if(w->l->color == BLACK){
				//w->r->color == RED
				w->r->color = BLACK;
				w->color = RED;
				_tree_rb_left_rotate(w);
				w = x->p->l;
			}else {
				//w->r->color == RED. w->l->color RED OR BLACK
				w->color = x->p->color;
				x->p->color = BLACK;
				w->l->color = BLACK;
				_tree_rb_left_rotate(x->p);
				//ajust root
				if(x->p == tree->root) tree->root = x->p->p;
				x = x->p->p;
			}
		}		
	}
	x->color = BLACK;	
}

//delete rb tree node, data will be free
int tree_rb_delete(tree_rb_t *tree, void const *data){
	if(NULL == tree) return -2;	
	tree_rb_node_t* ret_node;
	int find_ret = _tree_rb_find(tree, tree->root, data, NULL, &ret_node);
	if(0 != find_ret){
		return -1;	
	}
	if(ret_node->cnt > 1){
		ret_node->cnt --;
		return 0;
	}
	tree->num_node --;
	tree_rb_node_t *y, *x;
	if(ret_node->l == NULL || ret_node->r == NULL){
		y = ret_node;
	}else {
		//both child not empty, find last item of inorder iterations
		tree_rb_node_t *tmp = ret_node->l;
		while(tmp->r != NULL) tmp = tmp->r;
		y = tmp;
	}
	if(y->l == NULL) x = y->r;
	else x = y->l;

	x->p = y->p;
	if(x->p == NULL) tree->root = x;
	else {
		if(y == y->p->l) y->p->l = x;
		else y->p->r = x;
	}
	if(ret_node != y){
		//copy y data to ret_node
		ret_node->data = y->data;
		ret_node->cnt = y->cnt;		
	}
	
	if(y->color == BLACK){
		_tree_rb_delete_fix(tree, x);	
	}
	//free y
	/* !!!! free y's data */
	free(y->data);
	free(y);	
	return 0;
}
