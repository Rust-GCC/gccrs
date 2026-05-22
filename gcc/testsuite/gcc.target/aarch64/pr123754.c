/* PR rtl-optimization/123754 */
/* { dg-do compile } */
/* { dg-require-effective-target aarch64_big_endian } */
/* { dg-options "-O2 -favoid-store-forwarding" } */

typedef __attribute__((__vector_size__(32))) int V;
long long i;
V g;

void
foo (V *v)
{
  __builtin_memcpy (v, &i, 5);
  g *= *v;
}
