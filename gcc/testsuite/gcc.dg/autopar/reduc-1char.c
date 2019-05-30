/* { dg-do compile } */
/* { dg-options "-O2 -ftree-parallelize-loops=4 -fdump-tree-parloops2-details -fdump-tree-optimized" } */

#include <stdarg.h>
#include <stdlib.h>

#define N 1600
#define DIFF 242

unsigned char ub[N] = {1,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45};
unsigned char uc[N] = {1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

__attribute__ ((noinline)) void
main1 (unsigned char x, unsigned char max_result, unsigned char min_result)
{
  int i;
  unsigned char udiff = 2;
  unsigned char umax = x;
  unsigned char umin = x;

  for (i = 0; i < N; i++) {
    udiff += (unsigned char)(ub[i] - uc[i]);
  }

  for (i = 0; i < N; i++) {
    umax = umax < uc[i] ? uc[i] : umax;
  }

  for (i = 0; i < N; i++) {
    umin = umin > uc[i] ? uc[i] : umin;
  }

  /* check results:  */
  if (udiff != DIFF)
    abort ();
  if (umax != max_result)
    abort ();
  if (umin != min_result)
    abort ();
}

 __attribute__((noinline))
 void init_arrays ()
 {
   int i;

   for (i=16; i<N; i++)
     {
       ub[i] = 1;
       uc[i] = 1;
     }
}

int main (void)
{
  init_arrays();
  main1 (100, 100, 1);
  main1 (0, 15, 0);
  return 0;
}


/* { dg-final { scan-tree-dump-times "Detected reduction" 3 "parloops2" } } */
/* { dg-final { scan-tree-dump-times "SUCCESS: may be parallelized" 3 "parloops2" } } */

