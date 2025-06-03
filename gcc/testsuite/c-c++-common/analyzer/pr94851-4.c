/* { dg-additional-options "-O2 -Wno-analyzer-symbol-too-complex" } */
/* { dg-skip-if "requires hosted libstdc++ for stdlib free" { ! hostedlib } } */

#include <stdlib.h>

struct List {
    struct List *next;
};

void foo(struct List *p, struct List *q)
{
    while (p && p != q){
        struct List *next = p->next;
        free(p);
        p = next;
    }
}

int main()
{
    struct List x = {0};
    foo(NULL, &x);
    return 0;
}

