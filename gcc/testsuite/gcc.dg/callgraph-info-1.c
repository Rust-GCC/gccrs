/* { dg-do compile } */
/* { dg-options "-fcallgraph-info" } */
/* { dg-require-effective-target alias } */

void f() {}
void g() __attribute__ ((__alias__ ("f")));

/* { dg-final { scan-dump-times "ci" "triangle" 1 "ci" {{}} } } */
