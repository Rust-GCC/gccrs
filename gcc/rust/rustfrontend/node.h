#ifndef __NODE_H__
#define __NODE_H__

struct node {	
    struct node *next;	
    struct node *prev;	
    int own_string;	
    char const *name;
    int n_elems;	
    struct node** elems;	
};

extern struct node* mk_node(char const *name, int n, ...);
extern struct node* mk_atom(const char *name);
extern struct node* mk_none();
extern struct node* ext_node(struct node *nd, int n, ...);
extern void print_node(struct node *n, int depth);

extern int rslex();
extern void push_back(char);
extern void parser_init(int verbose);

extern int n_nodes;
extern node* nodes;

#endif //__NODE_H__
