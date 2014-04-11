#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xl_stack.h"
#include "xl_rbt.h"

#define RED 1
#define BLACK 2


xl_rbt_t* xl_rbt_init(xl_rbt_compare_fun_t *compare_fun){
	xl_rbt_t * tr = (xl_rbt_t*)calloc(sizeof(xl_rbt_t), 1);
	if(tr == NULL) return NULL;
	tr->root = NULL;
	tr->compare_fun = compare_fun;
	tr->num_node = 0;
	return tr;
}

int xl_rbt_free(xl_rbt_t *tree){
	if(NULL == tree) return 0;
	if(tree->root != NULL) return -1;
	free(tree);
	return 0;
}

int xl_rbt_destroy(xl_rbt_t *tree){
	if(NULL == tree) return 0;
	while(tree->root != NULL) {
		xl_rbt_delete(tree, tree->root->data);
	}
	free(tree);
	return 0;
}

static int _xl_rbt_find(xl_rbt_t* tree, xl_rbt_node_t *node, const void *data, 
		xl_rbt_node_t *p, xl_rbt_node_t **ret_node){
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
	}else if(compare_ret > 0){
		return _xl_rbt_find(tree, node->r, data, node, ret_node);
	}else {
		return _xl_rbt_find(tree, node->l, data, node, ret_node);
	}
}

int xl_rbt_find(xl_rbt_t *tree, const void *data, xl_rbt_node_t **ret_node){
	return _xl_rbt_find(tree, tree->root, data, NULL, ret_node);
}

/* left rotation */
static int _xl_rbt_left_rotate(xl_rbt_node_t *pivot){
	if(pivot == NULL || pivot->r == NULL) return -1;
	xl_rbt_node_t *tmp = pivot->r;
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
static int _xl_rbt_right_rotate(xl_rbt_node_t *pivot){
	if(pivot == NULL || pivot->l == NULL ) return -1;
	xl_rbt_node_t *tmp = pivot->l;
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

static void _xl_rbt_insert_fix(xl_rbt_t* tree, xl_rbt_node_t *new_node){
	while(new_node->p->color == RED){
		if(new_node->p == new_node->p->p->l){
			if(new_node->p->p->r != NULL && new_node->p->p->r->color == RED){
				new_node->p->p->r->color = BLACK;
				new_node->p->color = BLACK;
				new_node->p->p->color = RED;
				new_node = new_node->p->p;
			}else {
				if(new_node->p->r == new_node){
					new_node = new_node->p;
					//do not need to ajust root, cause pivot has parent
					_xl_rbt_left_rotate(new_node);
				}
				else {
					new_node->p->color = BLACK;
					new_node->p->p->color = RED; 	
					_xl_rbt_right_rotate(new_node->p->p);	
					//ajust root
					if(tree->root == new_node->p->r){
						tree->root = new_node->p;
					}
				}
			
			}
		}else {
			if(new_node->p->p->l != NULL && new_node->p->p->l->color == RED){
				new_node->p->p->l->color = BLACK;
				new_node->p->color = BLACK;
				new_node->p->p->color = RED;
				new_node = new_node->p->p;
			}else {
				if(new_node->p->l == new_node){
					new_node = new_node->p;
					_xl_rbt_right_rotate(new_node);
				}else {
					new_node->p->color = BLACK;
					new_node->p->p->color = RED;
					_xl_rbt_left_rotate(new_node->p->p);
					if(tree->root == new_node->p->l){
						tree->root = new_node->p;
					}
				}
			
			}
		}
		tree->root->color = BLACK;
		if(new_node == tree->root) break; 
	}	
}

int xl_rbt_insert(xl_rbt_t *tree, void *data){
	if(tree == NULL) return -2;
	xl_rbt_node_t *ret_node;
	int find_ret = _xl_rbt_find(tree, tree->root, data, NULL, &ret_node);
	if(0 == find_ret){
		ret_node->cnt ++;
		return 0;
	}
	//find fail, add node
	tree->num_node ++;
	xl_rbt_node_t *new_node = (xl_rbt_node_t*)calloc(sizeof(xl_rbt_node_t), 1);
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
	_xl_rbt_insert_fix(tree, new_node);
	return 0;
}

//delete fix
static void _xl_rbt_delete_fix(xl_rbt_t *tree, xl_rbt_node_t *x){
	xl_rbt_node_t *w;
	while(x->color == BLACK && x != tree->root){
		if(x == x->p->l){
			w = x->p->r;
			if(w != NULL && w->color == RED){
				w->color = BLACK;
				x->p->color = RED;
				_xl_rbt_left_rotate(x->p);
				//ajust root
				if(x->p == tree->root){
					tree->root = x->p->p;
				}
				w = x->p->r;
			}else if(w == NULL) {
				/*
				 *no such condition, cause if exists,
				 *x path has at least two B node, w path has only one B node
				 */
				printf("!!!!w is NULL\n");
			}
			if((w->l == NULL || w->l->color == BLACK) && (w->r == NULL || w->r->color == BLACK)){
				w->color = RED;
				x = x->p;
			}else if(w->r == NULL || w->r->color == BLACK){
				//w->l->color == RED
				if(w->l != NULL)
					w->l->color = BLACK;
				w->color = RED;
				_xl_rbt_right_rotate(w);
				w = x->p->r;
			}else{
				//w->r->color == RED, w->l->color RED OR BLACK
				w->color = x->p->color;
				x->p->color = BLACK;
				w->r->color = BLACK;
				_xl_rbt_left_rotate(x->p);
				//ajust root
				if(x->p == tree->root) tree->root = x->p->p;
				x = x->p->p;
			}
		}else {
			w = x->p->l;
			if(w != NULL && w->color == RED){
				w->color = BLACK;
				x->p->color = RED;
				_xl_rbt_right_rotate(x->p);
				//ajust root
				if(x->p == tree->root) tree->root = x->p->p;
				w = x->p->l;
			}else if(w == NULL){
			
				//not exist
				printf("!!!!w is NULL\n");
			}
			if((w->l == NULL || w->l->color == BLACK) && (w->r == NULL || w->r->color == BLACK)){
				w->color = RED;
				x = x->p;
			}else if(w->l == NULL || w->l->color == BLACK){
				//w->r->color == RED
				if(w->r != NULL)
					w->r->color = BLACK;
				w->color = RED;
				_xl_rbt_left_rotate(w);
				w = x->p->l;
			}else {
				//w->r->color == RED. w->l->color RED OR BLACK
				w->color = x->p->color;
				x->p->color = BLACK;
				w->l->color = BLACK;
				_xl_rbt_left_rotate(x->p);
				//ajust root
				if(x->p == tree->root) tree->root = x->p->p;
				x = x->p->p;
			}
		}		
	}
	x->color = BLACK;	
}

//delete rb tree node, data will be free
int xl_rbt_delete(xl_rbt_t *tree, const void *data){
	if(NULL == tree) return -2;	
	xl_rbt_node_t* ret_node;
	int find_ret = _xl_rbt_find(tree, tree->root, data, NULL, &ret_node);
	if(0 != find_ret){
		return -1;	
	}
	if(ret_node->cnt > 1){
		ret_node->cnt --;
		return 0;
	}
	tree->num_node --;
	xl_rbt_node_t *y, *x;
	if(ret_node->l == NULL || ret_node->r == NULL){
		y = ret_node;
	}else {
		//both child not empty, find last item of inorder iterations
		xl_rbt_node_t *tmp = ret_node->l;
		while(tmp->r != NULL) tmp = tmp->r;
		y = tmp;
	}
	if(y->l == NULL) x = y->r;
	else x = y->l;
	int sub_tag = 0;
	if(x == NULL){
		//construct a fake black node to substitude x
		sub_tag = 1;
		x = (xl_rbt_node_t*)calloc(sizeof(xl_rbt_node_t), 1);
		x->l = NULL;
		x->r = NULL;
		x->p = y;
		x->color = BLACK;
	} 
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
		_xl_rbt_delete_fix(tree, x);	
	}
	if(sub_tag == 1){
		if(x->p->l == x) x->p->l = NULL;
		else x->p->r = NULL;
		free(x);
	}
	//free y
	/* !!!! free y's data */
	free(y->data);
	free(y);	
	return 0;
}


void xl_rbt_inorder_traverse(xl_rbt_t *tree, xl_rbt_visit_fun_t *visit){
	if(tree->root == NULL) return;
	xl_stack_t * st = xl_stack_init(10, sizeof(char*));
	xl_rbt_node_t *node = tree->root;
	int64_t *tmp = 0;
	while(xl_stack_count(st) != 0 || node != NULL){
		if(node != NULL){
//			printf("node:0x%016lx\n", (int64_t)node);
			xl_stack_push(st, &node);
			node = node->l;
		}else {
			xl_stack_pop(st, (void **)&tmp);
			node = (xl_rbt_node_t *) (*tmp);
//			printf("poped node:0x%016lx\n", (int64_t)node);
			visit(node);
			node = node->r;
		}
///		xl_stack_status(st);
	}
	xl_stack_free(st);
}

void xl_rbt_preorder_traverse(xl_rbt_t *tree, xl_rbt_visit_fun_t *visit){
	if(tree->root == NULL) return;
	xl_stack_t *st = xl_stack_init(10, sizeof(char*));
	xl_stack_push(st, &(tree->root));
	xl_rbt_node_t *node = NULL;
	int64_t *tmp = 0;
	while(xl_stack_count(st) != 0){
		xl_stack_pop(st, (void **)&tmp);
		node = (xl_rbt_node_t *) (*tmp);
		visit(node);
		if(node->r != NULL) xl_stack_push(st, &(node->r));
		if(node->l != NULL) xl_stack_push(st, &(node->l));
	}
	xl_stack_free(st);
}

void xl_rbt_postorder_traverse(xl_rbt_t *tree, xl_rbt_visit_fun_t *visit){
	if(tree->root == NULL) return;
	xl_stack_t *st1 = xl_stack_init(10, sizeof(char*));
	xl_stack_t *st2 = xl_stack_init(10, sizeof(char*));
	xl_rbt_node_t *node = tree->root;
	xl_stack_push(st1, &node);
	int64_t *tmp = 0;
	while(node != NULL && xl_stack_count(st1) != 0){
		xl_stack_pop(st1, (void **)(&tmp));
		node = (xl_rbt_node_t*) (*tmp);
		xl_stack_push(st2, &node);
		if(node->l != NULL) xl_stack_push(st1, &(node->l));
		if(node->r != NULL) xl_stack_push(st1, &(node->r));
	}	
	while(xl_stack_count(st2) != 0){
		xl_stack_pop(st2, (void **)(&tmp));
		node = (xl_rbt_node_t*) (*tmp);
		visit(node);
	}
	xl_stack_free(st1);
	xl_stack_free(st2);
}
