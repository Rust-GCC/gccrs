/* { dg-do run { target gfni } } */
/* { dg-options "-mgfni -O2" } */

#define main do_test
#include "../../gcc.dg/builtin-bitreverse-1.c"
#undef main

int
main ()
{
  if (__builtin_cpu_supports ("gfni"))
    do_test ();
}
