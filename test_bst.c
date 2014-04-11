#include <stdlib.h>
#include <stdio.h>

#include "xl_bst.h"

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

void visit_st(const xl_bst_node_t *node){
	printf("visit:\t0x%016lx %d(%lu)\n", (int64_t)node, ((st*)(node->data))->key, node->cnt);
}

int
main(int argc, char* argv[]){
	xl_bst_t *tree = xl_bst_init(compare_st);
	
	int tmp;
	st *st_tmp;
	xl_bst_node_t *st_ret;
	int insert_ret, find_ret, delete_ret, rotate_ret;
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
			insert_ret = xl_bst_insert(tree, st_tmp);
			if(insert_ret != 0) {
				printf("insert error!\n");
			}
			xl_bst_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_postorder_traverse(tree, visit_st);
			printf("\n");
		}
		printf("find loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			find_ret = xl_bst_find(tree, st_tmp, &st_ret);
			if(find_ret != 0) {
				printf("find fail!\n");
			}
			free(st_tmp);
			xl_bst_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_postorder_traverse(tree, visit_st);
			printf("\n");
		}
		printf("delete loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			delete_ret = xl_bst_delete(tree, st_tmp);
			printf("root:0x%016lx\n", (int64_t)(tree->root));
			if(delete_ret != 0) {
				printf("delete fail!\n");
			}
			free(st_tmp);
			xl_bst_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_postorder_traverse(tree, visit_st);
			printf("\n");
		}
		printf("left rotate loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			rotate_ret = xl_bst_left_rotate(tree, st_tmp);
			printf("root:0x%016lx\n", (int64_t)(tree->root));
			if(rotate_ret != 0) {
				printf("left rotate fail!\n");
			}
			free(st_tmp);
			xl_bst_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_postorder_traverse(tree, visit_st);
			printf("\n");
		}

		printf("right rotate loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			rotate_ret = xl_bst_right_rotate(tree, st_tmp);
			printf("root:0x%016lx\n", (int64_t)(tree->root));
			if(rotate_ret != 0) {
				printf("right rotate fail!\n");
			}
			free(st_tmp);
			xl_bst_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_bst_postorder_traverse(tree, visit_st);
			printf("\n");
		}

	}
}
