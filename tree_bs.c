/* 
 * tree_bs.c binary search tree
 * paco, Mar. 3rd, 2014
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_bs.h"
#include "stack.h"

tree_bs_t *tree_bs_init(tree_bs_compare_fun_t* compare_fun){
	tree_bs_t * tr = (tree_bs_t *) calloc(sizeof(tree_bs_t), 1);
	if(tr == NULL) return NULL;
	tr->root = NULL;

	tr->compare_fun = compare_fun;
	tr->num_node = 0;
	
	return tr;
}
//do not free tr when root is NOT NULL
int tree_bs_free(tree_bs_t *tr){
	if(tr == NULL) return 0;
	if(tr->root != NULL) return -1;
	free(tr);
	return 0;
}
//if root is not NULL delete every node, then free tree
int tree_bs_destroy(tree_bs_t *tree){
	if(tree == NULL) return 0;
	while(tree->root != NULL){
		tree_bs_delete(tree, tree->root->data);
	}
	free(tree);
	return 0;
}
/* 
 * search tree, 
 * when seearch success, ret_p_node is the node visited before the target
 * ret_node is the target node
 * whern fail, ret_p_node and ret_node are all point to the last visited node
 */
static int _tree_bs_find(tree_bs_t *tree, tree_bs_node_t *node, const void* data, 
		tree_bs_node_t *p, tree_bs_node_t **ret_p_node, tree_bs_node_t **ret_node){
	if(tree == NULL) return -2;
	if(node == NULL) {
		if(ret_p_node != NULL)
			*ret_p_node = p;
		if(ret_node != NULL)
			*ret_node = p;
		return -1;
	}
	
	/*compare the key*/
	int ret = tree->compare_fun(node->data, data);
	if(ret == 0) {
		if(ret_p_node != NULL)
			*ret_p_node = p;
		if(ret_node != NULL)
			*ret_node = node;
		return 0;
	}else if(ret < 0){
		return _tree_bs_find(tree, node->r, data, node, ret_p_node, ret_node);
	} else {
		return _tree_bs_find(tree, node->l, data, node, ret_p_node, ret_node);
	}
}

int tree_bs_find(tree_bs_t *tree, const void* data, tree_bs_node_t **ret_node){
	if(tree == NULL) return -1;
    return _tree_bs_find(tree, tree->root, data, NULL, NULL, ret_node);	
}

/* insert */
int tree_bs_insert(tree_bs_t *tree, void* data){
	if(tree == NULL) return -1;
	if(data == NULL) return -1;
	tree_bs_node_t *ret_node_p, *ret_node;
	int find_ret = _tree_bs_find(tree, tree->root, data, NULL, &ret_node_p, &ret_node);
	//find success, add cnt
	if(0 == find_ret){
		ret_node->cnt ++;	
	}else {
		tree_bs_node_t * new_node = (tree_bs_node_t*)calloc(sizeof(tree_bs_node_t), 1);
	//	printf("new node: 0x%016lx\n", (int64_t) new_node);	
		new_node->cnt = 1;
		new_node->l = NULL;
		new_node->r = NULL;
		//asign data
		new_node->data = data; 
		//root is NULL
		if(tree->root == NULL) {
			tree->root = new_node;
		}else{
			int compare_ret = tree->compare_fun(ret_node->data, data);
			if(compare_ret < 0){
				ret_node->r = new_node;
			}else ret_node->l = new_node;
		}
		tree->num_node ++;
	}
	return 0;
}

/*delete*/
int tree_bs_delete(tree_bs_t *tree, const void *data){
	if(tree == NULL) return -1;
	if(tree->root == NULL) return 0;
	tree_bs_node_t *ret_node_p, *ret_node;
	int find_ret = _tree_bs_find(tree, tree->root, data, NULL, &ret_node_p, &ret_node);
	//find fail
	if(0 != find_ret){
		return -2;	
	}
	//printf("ret_node_p:0x%016lx\tret_node:0x%016lx\n", (int64_t)ret_node_p, (int64_t)ret_node);
	if(ret_node->cnt > 1) {
		ret_node->cnt --;
		return 0;
	}
	tree->num_node --;
	//both children is NULL
	if(ret_node->l == NULL && ret_node->r == NULL) {
		if(ret_node_p != NULL) {
			if(ret_node_p->l == ret_node) ret_node_p->l = NULL;
			else ret_node_p->r = NULL;
		}else {
			//target node is root
			tree->root = NULL;
		}
		free(ret_node->data);
		free(ret_node);
		return 0;
	}
	if(ret_node->l != NULL && ret_node->r != NULL){
		tree_bs_node_t *tmp_p = ret_node;
		tree_bs_node_t *tmp = ret_node->l;	
		//find ret_node's prevois node of inorder traverse sequence 
		while(tmp->r != NULL) {
			tmp_p = tmp;
			tmp = tmp->r;
		}
		free(ret_node->data);	
		ret_node->data = tmp->data;	
		ret_node->cnt = tmp->cnt;
		if(tmp_p == ret_node){
			ret_node->l = tmp->l; 
		}else{
			tmp_p->r = tmp->l;
		}
		free(tmp);
		return 0;
	}
	tree_bs_node_t * tmp_node;
	if(ret_node->l == NULL) tmp_node = ret_node->r;
	else tmp_node = ret_node->l;
	
	if(ret_node_p != NULL) {
		if(ret_node_p->l == ret_node){
			ret_node_p->l = tmp_node;
		}else ret_node_p->r = tmp_node;
	}else {
		//ret_node is root
		tree->root = tmp_node;
	}
	free(ret_node->data);
	free(ret_node);
	return 0;
}

/* left rotation */
//p: pivot's parent
static int _tree_bs_left_rotate(tree_bs_node_t *pivot, tree_bs_node_t *p, tree_bs_node_t **new_root){
	if(pivot == NULL || pivot->r == NULL) return -1;
	tree_bs_node_t *tmp = pivot->r;
	pivot->r = tmp->l;
	tmp->l = pivot;
	if(p != NULL){
		if(p->l == pivot) p->l = tmp;
		else p->r = tmp;
	}
	if(new_root != NULL) *new_root = tmp;
	return 0;	
}  

int tree_bs_left_rotate(tree_bs_t *tree, const void *data){
	if(tree == NULL || tree->root == NULL || data == NULL) return -1;
	tree_bs_node_t *ret_node, *ret_p_node, *new_root;
	int find_ret = _tree_bs_find(tree, tree->root, data, NULL, &ret_p_node, &ret_node);
	if(find_ret != 0) return -2;//not find
	//rotate
	int rotate_ret = _tree_bs_left_rotate(ret_node, ret_p_node, &new_root);
	if(rotate_ret != 0) return -3;//rotate fail
	//pivot is root
	if(ret_p_node == NULL && ret_node == tree->root) {
		tree->root = new_root;
	}
	return 0;
}

/* right rotate */
//p:pivot's parent
static int _tree_bs_right_rotate(tree_bs_node_t *pivot, tree_bs_node_t *p, tree_bs_node_t **new_root){
	if(pivot == NULL || pivot->l == NULL ) return -1;
	tree_bs_node_t *tmp = pivot->l;
	pivot->l = tmp->r;
	tmp->r = pivot;
	if(p != NULL){
		if(p->l == pivot) p->l = tmp;
		else p->r = tmp;
	}
	if(new_root != NULL) *new_root = tmp;
	return 0;
}

int tree_bs_right_rotate(tree_bs_t *tree, const void *data){
	if(tree == NULL || tree->root == NULL || data == NULL) return -1;
	tree_bs_node_t *ret_node, *ret_p_node, *new_root;
	int find_ret = _tree_bs_find(tree, tree->root, data, NULL, &ret_p_node, &ret_node);
	if(find_ret != 0) return -2;
	
	//rotate
	int rotate_ret = _tree_bs_right_rotate(ret_node, ret_p_node, &new_root);
	if(rotate_ret != 0) return -3;

	//pivot is root
	if(ret_p_node == NULL && ret_node == tree->root){
		tree->root = new_root;
	}
	return 0;
}
/*  
static void _tree_bs_inorder_traverse(tree_bs_node_t *node, visit_fun_t *visit){
	if(node == NULL) return ;
	_tree_bs_inorder_traverse(node->l, visit);
	visit(node);
	_tree_bs_inorder_traverse(node->r, visit);
}
*/
void tree_bs_inorder_traverse(tree_bs_t *tree, tree_bs_visit_fun_t *visit){
//	_tree_bs_inorder_traverse(tree->root, visit);
	//no recusive implent
	if(tree->root == NULL) return;
	stack_t * st = stack_init(10, sizeof(char*));
	tree_bs_node_t *node = tree->root;
	int64_t *tmp = 0;
	while(stack_count(st) != 0 || node != NULL){
		if(node != NULL){
//			printf("node:0x%016lx\n", (int64_t)node);
			stack_push(st, &node);
			node = node->l;
		}else {
			stack_pop(st, (void **)&tmp);
			node = (tree_bs_node_t *) (*tmp);
//			printf("poped node:0x%016lx\n", (int64_t)node);
			visit(node);
			node = node->r;
		}
///		stack_status(st);
	}
	stack_free(st);
}
/*
static void _tree_bs_preorder_traverse(tree_bs_node_t *node, visit_fun_t *visit){
	if(node == NULL) return ;
	visit(node);
	_tree_bs_preorder_traverse(node->l, visit);
	_tree_bs_preorder_traverse(node->r, visit);
}
*/
void tree_bs_preorder_traverse(tree_bs_t *tree, tree_bs_visit_fun_t *visit){
	//_tree_bs_preorder_traverse(tree->root, visit);
	if(tree->root == NULL) return;
	stack_t *st = stack_init(10, sizeof(char*));
	stack_push(st, &(tree->root));
	tree_bs_node_t *node = NULL;
	int64_t *tmp = 0;
	while(stack_count(st) != 0){
		stack_pop(st, (void **)&tmp);
		node = (tree_bs_node_t *) (*tmp);
		visit(node);
		if(node->r != NULL) stack_push(st, &(node->r));
		if(node->l != NULL) stack_push(st, &(node->l));
	}
	stack_free(st);
}
/*
static void _tree_bs_postorder_traverse(tree_bs_node_t *node, visit_fun_t *visit){
	if(node == NULL) return ;
	_tree_bs_postorder_traverse(node->l, visit);
	_tree_bs_postorder_traverse(node->r, visit);
	visit(node);
}
*/
void tree_bs_postorder_traverse(tree_bs_t *tree, tree_bs_visit_fun_t *visit){
//	_tree_bs_postorder_traverse(tree->root, visit);
/*  
	if(tree->root == NULL) return;
	stack_t *st1 = stack_init(10, sizeof(char*));
	stack_t *st2 = stack_init(10, sizeof(char*));
	tree_bs_node_t *node = tree->root;
	int64_t *tmp = 0;
	int flag = 0;
	stack_push(st1, &(tree->root));
	while(node != NULL){
		while(node != NULL){
			if(node->r != NULL) stack_push(st1, &(node->r));
			if(node->l != NULL) stack_push(st2, &(node->l));
			node = node->r;
		}
		flag = 0;
		while(stack_count(st2) != 0){
			flag = 1;
			stack_pop(st2, (void**) (&tmp));
			stack_push(st1, tmp);	
		}
		if(1 == flag){	
			stack_peek(st1, (void **)(&tmp));
			node = (tree_bs_node_t*) (*tmp);
		}
	}
	while(stack_count(st1) != 0){
		stack_pop(st1, (void**) (&tmp));
		node = (tree_bs_node_t*) (*tmp);
		visit(node);
	}
	stack_free(st1);
	stack_free(st2);
*/
	if(tree->root == NULL) return;
	stack_t *st1 = stack_init(10, sizeof(char*));
	stack_t *st2 = stack_init(10, sizeof(char*));
	tree_bs_node_t *node = tree->root;
	stack_push(st1, &node);
	int64_t *tmp = 0;
	while(node != NULL && stack_count(st1) != 0){
		stack_pop(st1, (void **)(&tmp));
		node = (tree_bs_node_t*) (*tmp);
		stack_push(st2, &node);
		if(node->l != NULL) stack_push(st1, &(node->l));
		if(node->r != NULL) stack_push(st1, &(node->r));
	}	
	while(stack_count(st2) != 0){
		stack_pop(st2, (void **)(&tmp));
		node = (tree_bs_node_t*) (*tmp);
		visit(node);
	}
	stack_free(st1);
	stack_free(st2);
}

