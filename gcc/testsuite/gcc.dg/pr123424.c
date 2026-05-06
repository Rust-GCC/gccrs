/* { dg-do compile } */
/* { dg-options "-std=c23" } */

void bar(const struct S *x, int y) {	/* { dg-warning "not be visible" } */
  const struct S {
    int d[y];
  } *a = x;
};

