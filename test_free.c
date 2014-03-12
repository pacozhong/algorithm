#include <stdlib.h>
#include <stdio.h>


typedef struct t_str
{
	char a;
	char *b;
	char c;
} t_str;

char a_str[] = "abc";
char b_str[] = "bbc";


int change(t_str *a, char** b){

	a->a = 'c';
	a->b = b_str;
	a->c = 'a';
	*b = "aaa";
	printf("%c\t0x%016lx\t%c\t%s\n", a->a, (int64_t)a->b, a->c, *b);
	return 0;
}

int
main(int argc, char* argv[]){

	t_str *a = (t_str*)malloc(sizeof(t_str));
	a->a = 'a';
	a->b = a_str;
	a->c = 'c';
	
	char* b = "bbb";
	

	printf("%c\t0x%016lx\t%c\t%s\n", a->a, (int64_t)a->b, a->c, b);
	change(a, &b);
	printf("%c\t0x%016lx\t%c\t%s\n", a->a, (int64_t)a->b, a->c, b);
	return 0;
}
