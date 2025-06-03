/* { dg-do compile } */
/* { dg-final { scan-assembler-times {\tveval\t%v[0-9]+,%v[0-9]+,%v[0-9]+,%v[0-9]+,31} 8 } } */

void
and_ior (char *res, char *x, char *y, char *z)
{
  for (int i = 0; i < 128; ++i)
    res[i] = (x[i] & y[i]) | z[i];
}
