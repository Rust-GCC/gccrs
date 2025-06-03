/* { dg-do run { target { powerpc*-*-* && vmx_hw } } } */
/* { dg-do compile { target { powerpc*-*-* && { ! vmx_hw } } } } */
/* { dg-options "-maltivec" } */
/* { dg-require-effective-target powerpc_altivec } */

#include <altivec.h>

extern void abort (void);

typedef int v4si __attribute__ ((vector_size (16)));

#define MAGIC_NUMBER 12345

v4si my_vect;
int my_array[4] __attribute__ ((aligned (16)));

void initialize (int a)
{
  my_vect = (v4si) {0, a, 2, 3};
  vec_st (my_vect, 0, my_array);
}

int verify (void)
{
  if (my_array[1] != MAGIC_NUMBER)
    abort ();
}

int main (void)
{
  initialize (MAGIC_NUMBER);
  verify ();
  return 0;
}

