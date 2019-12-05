#include "rust-system.h"
#include "node.h"


static int const indent_step = 4;

void print_indent(int depth);

#define PUSHBACK_LEN 4	

static char pushback[PUSHBACK_LEN];	
static int verbose;
struct node* nodes = NULL;
int n_nodes;

extern int yylex();

void
parser_init(int v)
{
    verbose = v;
    n_nodes = 0;
    memset(pushback, '\0', PUSHBACK_LEN);
}

// If there is a non-null char at the head of the pushback queue,	
// dequeue it and shift the rest of the queue forwards. Otherwise,	
// return the token from calling yylex.	
int
rslex()
{	
    if (pushback[0] == '\0') {	
        return yylex();	
    } else {	
        char c = pushback[0];	
        memmove(pushback, pushback + 1, PUSHBACK_LEN - 1);	
        pushback[PUSHBACK_LEN - 1] = '\0';	
        return c;	
    }
}	

// Note: this does nothing if the pushback queue is full. As long as	
// there aren't more than PUSHBACK_LEN consecutive calls to push_back	
// in an action, this shouldn't be a problem.	
void push_back(char c)
{	
    for (int i = 0; i < PUSHBACK_LEN; ++i) {	
        if (pushback[i] == '\0') {	
            pushback[i] = c;	
            break;	
        }	
    }	
}	


struct node*
mk_node(char const *name, int n, ...)
{
    struct node* nd = (struct node *)xmalloc(sizeof(struct node));
    nd->elems = (struct node**) xcalloc(n, sizeof(struct node *));

    printf("# New %d-ary node: %s = %p\n", n, name, nd);	

    nd->own_string = 0;	
    nd->prev = NULL;	
    nd->next = nodes;	
    if (nodes) {	
        nodes->prev = nd;	
    }	
    nodes = nd;	

    nd->name = name;	
    nd->n_elems = n;
    
    int i = 0;
    struct node *nn;

    va_list ap;	
    va_start(ap, n);	
    while (i < n) {	
        nn = va_arg(ap, struct node *);	
        printf("#   arg[%d]: %p\n", i, nn);	
        printf("#            (%s ...)\n", nn->name);	
        nd->elems[i] = nn;
        i++;
    }	
    va_end(ap);	
    n_nodes++;	
    return nd;	
}	

struct node*
mk_atom(const char *name) {	
    struct node *nd = mk_node(xstrdup(name), 0);
    nd->own_string = 1;	
    return nd;	
}	

struct node*
mk_none() {	
    return mk_atom("<none>");	
}	

struct node*
ext_node(struct node *nd, int n, ...) {	
    va_list ap;	
    int i = 0, c = nd->n_elems + n;	
    unsigned sz = sizeof(struct node) + (c * sizeof(struct node *));	
    struct node *nn;	

    printf("# Extending %d-ary node by %d nodes: %s = %p",	
          nd->n_elems, c, nd->name, nd);	

    if (nd->next) {	
        nd->next->prev = nd->prev;	
    }	
    if (nd->prev) {	
        nd->prev->next = nd->next;	
    }	
    nd = (struct node*)xrealloc(nd, sz);
    nd->prev = NULL;	
    nd->next = nodes;	
    nodes->prev = nd;	
    nodes = nd;	

    printf(" ==> %p\n", nd);	

    va_start(ap, n);	
    while (i < n) {	
        nn = va_arg(ap, struct node *);	
        printf("#   arg[%d]: %p\n", i, nn);	
        printf("#            (%s ...)\n", nn->name);	
        nd->elems[nd->n_elems++] = nn;	
        ++i;	
    }	
    va_end(ap);	
    return nd;	
}	



void
print_indent(int depth) {
    while (depth) {	
        if (depth-- % indent_step == 0) {	
            printf("|");	
        } else {	
            printf(" ");	
        }	
    }	
}	

void
print_node(struct node *n, int depth) {	
    int i = 0;	
    print_indent(depth);	
    if (n->n_elems == 0) {	
        printf("%s\n", n->name);	
    } else {	
        printf("(%s\n", n->name);	
        for (i = 0; i < n->n_elems; ++i) {	
            print_node(n->elems[i], depth + indent_step);	
        }	
        print_indent(depth);	
        printf(")\n");	
    }
}
