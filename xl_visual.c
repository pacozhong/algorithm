#include <stdio.h>
#include <stdlib.h>

#include "xl_bst.h"
#include "xl_rbt.h"
#include "xl_visual.h"

static void _xl_visual_bst_func(const xl_bst_node_t* node, void *data){
	if(node == NULL) return;
	
	static int null_sequence = 0;
	FILE *fd = (FILE*) (*((char**)data));
	xl_get_key_fun_t *get_key = (xl_get_key_fun_t*) *((char**)data + 1);
	int key = get_key(node->data);
	fprintf(fd, "node%d[label = \"%d(%lu)\"];\n", key, key, node->cnt);
	if(node->l){
		fprintf(fd, "node%d->node%d;\n", key, get_key(node->l->data));
	}else {
		fprintf(fd, "node%d->null%d;\n", key, null_sequence);
		fprintf(fd, "null%d[label = \"null\"];\n",null_sequence ++);
	}
	if(node->r){
		fprintf(fd, "node%d->node%d;\n", key, get_key(node->r->data));
	}else {
		fprintf(fd, "node%d->null%d;\n", key, null_sequence);
		fprintf(fd, "null%d[label = \"null\"];\n",null_sequence ++);
	}
}

int xl_visual_bst(xl_bst_t *bst, char* filename, xl_get_key_fun_t *fun){
	FILE *fd = fopen(filename, "w");
	if(fd == NULL) return -1;
	char **data = (char **) malloc(sizeof(char*) * 2);
	*data = (char*)fd;
	*(data + 1) = (char*)fun;
	fprintf(fd, "digraph G{\n");
	if(bst->root != NULL)
		fprintf(fd, "root->node%d;\n", fun(bst->root->data));
	xl_bst_preorder_traverse(bst, _xl_visual_bst_func, data);
	fprintf(fd, "}");	
	free(data);
	fclose(fd);
	return 0;
}

static void _xl_visual_rbt_func(const xl_rbt_node_t* node, void *data){
	if(node == NULL) return;
	
	static int null_sequence = 0;
	FILE *fd = (FILE*) (*((char**)data));
	xl_get_key_fun_t *get_key = (xl_get_key_fun_t*) *((char**)data + 1);
	int key = get_key(node->data);
	fprintf(fd, "node%d[label = \"%d(%lu)\", color = %s];\n", 
			key, key, node->cnt, node->color == XL_BLACK ? "black" : "red");
	if(node->l){
		fprintf(fd, "node%d->node%d;\n", key, get_key(node->l->data));
	}else {
		fprintf(fd, "node%d->null%d;\n", key, null_sequence);
		fprintf(fd, "null%d[label = \"null\", color = black];\n",null_sequence ++);
	}
	if(node->r){
		fprintf(fd, "node%d->node%d;\n", key, get_key(node->r->data));
	}else {
		fprintf(fd, "node%d->null%d;\n", key, null_sequence);
		fprintf(fd, "null%d[label = \"null\", color = black];\n",null_sequence ++);
	}
}


int xl_visual_rbt(xl_rbt_t *rbt, char* filename, xl_get_key_fun_t *fun){
	FILE *fd = fopen(filename, "w");
	if(fd == NULL) return -1;
	char **data = (char **) malloc(sizeof(char*) * 2);
	*data = (char*)fd;
	*(data + 1) = (char*)fun;
	fprintf(fd, "digraph G{\n");
	if(rbt->root != NULL)
		fprintf(fd, "root->node%d;\n", fun(rbt->root->data));
	xl_rbt_preorder_traverse(rbt, _xl_visual_rbt_func, data);
	fprintf(fd, "}");	
	free(data);
	fclose(fd);
	return 0;
}
