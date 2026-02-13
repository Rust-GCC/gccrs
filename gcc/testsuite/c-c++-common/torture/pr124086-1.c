/* { dg-do compile } */
/* { dg-options "" } */
/* PR tree-optimization/124086 */


typedef __attribute__((__vector_size__(2*sizeof(long double)))) int V;
int j;

void
foo()
{
  V v = (V){0, -0xd};
  _Complex long double t =  *(_Complex long double *)&v;
  j = __real__ t;
}
