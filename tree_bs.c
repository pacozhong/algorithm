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

tree_bs_t *tree_bs_init(compare_fun_t* compare_fun){
	tree_bs_t * tr = (tree_bs_t *) calloc(sizeof(tree_bs_t), 1);
	tr->root = NULL;

	tr->compare_fun = compare_fun;
	tr->num_node = 0;
	
	return tr;
}

int tree_bs_free(tree_bs_t *tr){
	if(tr == NULL) return 0;
	if(tr->root != NULL) return -1;
	free(tr);
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
		*ret_node = p;
		return -1;
	}
	
	/*compare the key*/
	int ret = tree->compare_fun(node->data, data);
	if(ret == 0) {
		if(ret_p_node != NULL)
			*ret_p_node = p;
		*ret_node = node;
		return 0;
	}else if(ret < 0){
		return _tree_bs_find(tree, node->r, data, node, ret_p_node, ret_node);
	} else {
		return _tree_bs_find(tree, node->l, data, node, ret_p_node, ret_node);
	}
}

int tree_bs_find(tree_bs_t *tree, const void* data, tree_bs_node_t **ret_node){
	if(tree->root == NULL) return -1;
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
	ret_node->cnt --;
	if(ret_node->cnt > 0) return 0;
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
		ret_node->data = tmp->data;	
		ret_node->cnt ++;
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

void _tree_bs_inorder_traverse(tree_bs_node_t *node, visit_fun_t *visit){
	if(node == NULL) return ;
	_tree_bs_inorder_traverse(node->l, visit);
	visit(node);
	_tree_bs_inorder_traverse(node->r, visit);
}

void tree_bs_inorder_traverse(tree_bs_t *tree, visit_fun_t *visit){
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

void _tree_bs_preorder_traverse(tree_bs_node_t *node, visit_fun_t *visit){
	if(node == NULL) return ;
	visit(node);
	_tree_bs_preorder_traverse(node->l, visit);
	_tree_bs_preorder_traverse(node->r, visit);
}

void tree_bs_preorder_traverse(tree_bs_t *tree, visit_fun_t *visit){
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

void _tree_bs_postorder_traverse(tree_bs_node_t *node, visit_fun_t *visit){
	if(node == NULL) return ;
	_tree_bs_postorder_traverse(node->l, visit);
	_tree_bs_postorder_traverse(node->r, visit);
	visit(node);
}

void tree_bs_postorder_traverse(tree_bs_t *tree, visit_fun_t *visit){
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

