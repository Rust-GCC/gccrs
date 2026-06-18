/* { dg-do compile } */
/* { dg-options "-O2" } */
/* PR tree-optimization/125857 */

/* Make sure we don't ICE when returning the address to a local variable
   and using it in the other function.  */
int *a, **b = &a, c;
int *d() {
  int e, *f = &e;
  for (; c; c++)
    __builtin_abort();
  if (!b)
    __builtin_abort();
  return f; /* { dg-warning "function may return address of local variable" } */
}
int main() { *b = d(); }
