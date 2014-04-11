
#include <stdio.h>
#include <stdlib.h>
#include "xl_util.h"
#include "xl_rbt.h"
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

void visit_st(const xl_rbt_node_t *node){
	char *a = (char*)malloc(sizeof(char) * 17);
	printf("visit:\t%s %d(%d)%c\n", xl_print_addr(node, a), ((st*)(node->data))->key, node->cnt, node->color == 1 ? 'R' : 'B');
	free(a);
}

int
main(int argc, char* argv[]){
	xl_rbt_t *tree = xl_rbt_init(compare_st);
	
	int tmp;
	st *st_tmp;
	xl_rbt_node_t *st_ret;
	int insert_ret, find_ret, delete_ret;
	//insert loop
	while(1){
		printf("insert loop:\n");
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			printf("st_tmp:");
			xl_print_addr(st_tmp, NULL);
			printf("\n");
			printf("key:%d\n", st_tmp->key);
			insert_ret = xl_rbt_insert(tree, st_tmp);
			if(insert_ret != 0) {
				printf("insert error!\n");
			}
			xl_rbt_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_rbt_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_rbt_postorder_traverse(tree, visit_st);
			printf("\n");
		}
		printf("find loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			find_ret = xl_rbt_find(tree, st_tmp, &st_ret);
			if(find_ret != 0) {
				printf("find fail!\n");
			}
			free(st_tmp);
			xl_rbt_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_rbt_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_rbt_postorder_traverse(tree, visit_st);
			printf("\n");
		}
		printf("delete loop:\n");
		//search loop
		while(scanf("%d", &tmp) != EOF){
			st_tmp = (st*)malloc(sizeof(st));
			st_tmp->a = "a";
			st_tmp->key = tmp;
			st_tmp->pad = "pad";
			delete_ret = xl_rbt_delete(tree, st_tmp);
			printf("root:");
			xl_print_addr(tree->root, NULL);
			printf("\n");
			if(delete_ret != 0) {
				printf("delete fail!\n");
			}
			free(st_tmp);
			xl_rbt_inorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_rbt_preorder_traverse(tree, visit_st);
			printf("////////////\n");
			xl_rbt_postorder_traverse(tree, visit_st);
			printf("\n");
		}
	}
}
