/* { dg-do run } */
/* { dg-skip-if "" { *-*-* } { "-O0" } } */
/* { dg-additional-options "-fisolate-erroneous-paths-dereference" } */

int a;
static void __attribute__((noipa)) c(int b) {}
static int * __attribute__((noipa))
d()
{
  int g = 0, *h = &g;
  if (a)
    {
      int **i = &h;
      *i = 0;
    }
  c(*h);
  return h; /* { dg-warning "address of local variable" } */
}
int
main()
{
  d();
  return 0;
}
