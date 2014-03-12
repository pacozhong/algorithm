#include <stdlib.h>
#include <stdio.h>

#include "tree_bs.h"

/* test code */

typedef struct st {
	char * a;
	int key;
	char*  pad;
}st;

int compare_st(const void *a, const void *b){
//	printf("a:0x%016lx\tb:0x%016lx\n", (int64_t)a, (int64_t)b);
	return ((st*)a)->key - ((st*)b)->key;
}

void visit_st(const tree_bs_node_t *node){
	printf("visit:\t0x%016lx %d(%lu)\n", (int64_t)node, ((st*)(node->data))->key, node->cnt);
}

int
main(int argc, char* argv[]){
	tree_bs_t *tree = tree_bs_init(compare_st);
	
	int tmp;
	st *st_tmp;
	tree_bs_node_t *st_ret;
	int insert_ret, find_ret, delete_ret;
	//insert loop
	while(1){
		printf("insert loop:\n");
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			printf("st_tmp:0x%016lx\n", (int64_t)st_tmp);
			printf("key:%d\n", st_tmp->key);
			insert_ret = tree_bs_insert(tree, st_tmp);
			if(insert_ret != 0) {
				printf("insert error!\n");
			}
			tree_bs_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			tree_bs_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			tree_bs_postorder_traverse(tree, visit_st);
			printf("\n");
		}
		printf("find loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			find_ret = tree_bs_find(tree, st_tmp, &st_ret);
			if(find_ret != 0) {
				printf("find fail!\n");
			}
			free(st_tmp);
			tree_bs_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			tree_bs_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			tree_bs_postorder_traverse(tree, visit_st);
			printf("\n");
		}
		printf("delete loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			delete_ret = tree_bs_delete(tree, st_tmp);
			printf("root:0x%016lx\n", (int64_t)(tree->root));
			if(delete_ret != 0) {
				printf("delete fail!\n");
			}
			free(st_tmp);
			tree_bs_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			tree_bs_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			tree_bs_postorder_traverse(tree, visit_st);
			printf("\n");
		}
	}
}
