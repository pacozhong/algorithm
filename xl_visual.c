#include <stdio.h>
#include <stdlib.h>

#include "xl_visual.h"

static void xl_visual_bst_func(const xl_bst_node_t* node, void *data){
	if(node == NULL) return;
	FILE *fd = (FILE*) *data;
	xl_get_key_fun_t *get_key = (xl_get_key_fun_t*) *(data + 1);
	int key = get_key(node->data);
	fprintf(fd, "node%d[label = \"%d(%lu);\"]\n", key, key, node->cnt);
	if(node->l){
		fprintf(fd, "node%d->node%d;\n", key, get_key(node->l->data))
	}
	if(node->r){
		fprintf(fd, "node%d->node%d;\n", key, get_key(node->r->data))
	}
}

int xl_visual_bst(xl_bst_t *bst, char* filename, xl_get_key_fun_t *fun){
	FILE *fd = fopen(filename, "w");
	if(fd == NULL) return -1;
	char **data = (char **) malloc(sizeof(char*) * 2);
	*data = fd;
	*(data + 1) = fun;
	fprintf(fd, "digraph G{\n");
	if(bst->root != NULL)
		fprintf(fd, "root->node%d;\n", fun(bst->root->data));
	xl_bst_preorder_traverse(bst, xl_visual_bst_func, data);
	fprintf(fd, "}");	
	free(data);
	fclose(fd);
}
