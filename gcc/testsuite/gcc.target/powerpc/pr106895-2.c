/* PR target/106895 */
/* { dg-do run } */
/* { dg-require-effective-target int128 } */
/* { dg-options "-O2" } */

#include <stdlib.h>

typedef __int128 ti;
typedef __int128 pti __attribute__((mode(PTI)));

volatile ti  a = 140;
volatile pti b;
volatile ti  c;

int main(void)
{
  b = (pti) a;
  c = (ti) b;

  if (c != a)
    abort();

  return 0;
}
