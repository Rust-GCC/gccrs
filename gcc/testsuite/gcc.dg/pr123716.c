/* PR c/123716 */
/* { dg-do compile } */
/* { dg-options "-O2" } */
int f(unsigned s)
{
  int i;
  for (i = 0; i < s; ++i);
  unsigned i[1];  /* { dg-error "conflicting type" } */
}
