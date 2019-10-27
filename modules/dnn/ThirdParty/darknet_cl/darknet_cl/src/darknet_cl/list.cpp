#include <stdlib.h>
#include <string.h>
#include "list.h"

listDark *make_list()
{
	listDark *l = (list*)malloc(sizeof(listDark));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	return l;
}

/*
void transfer_node(listDark *s, listDark *d, node *n)
{
    node *prev, *next;
    prev = n->prev;
    next = n->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;
    --s->size;
    if(s->front == n) s->front = next;
    if(s->back == n) s->back = prev;
}
*/

char *list_pop(listDark *l){
    if(!l->back) return 0;
    node *b = l->back;
    char *val = b->val;
    l->back = b->prev;
    if(l->back) l->back->next = 0;
    free(b);
    --l->size;
    
    return val;
}

void list_insert(listDark *l, char *val)
{
	node *newnode = (node*)malloc(sizeof(node));
	newnode->val = val;
	newnode->next = 0;

	if(!l->back){
		l->front = newnode;
		newnode->prev = 0;
	}else{
		l->back->next = newnode;
		newnode->prev = l->back;
	}
	l->back = newnode;
	++l->size;
}

void free_node(node *n)
{
	node *next;
	while(n) {
		next = n->next;
		free(n);
		n = next;
	}
}

void free_list(listDark *l)
{
	free_node(l->front);
	free(l);
}

void free_list_contents(listDark *l)
{
	node *n = l->front;
	while(n){
		free(n->val);
		n = n->next;
	}
}

char **list_to_array(listDark *l)
{
    char **a = (char**)calloc(l->size, sizeof(char*));
    int count = 0;
    node *n = l->front;
    while(n){
        a[count++] = n->val;
        n = n->next;
    }
    return a;
}
