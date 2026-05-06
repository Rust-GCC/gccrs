/* { dg-do run } */
/* { dg-require-effective-target offload_device_nonshared_as } */

/* This testcase used to ICE.  Test that an array can appear more than once
   in map clauses in combination with iterators.  */

#include <stdlib.h>

#define DIM 128

void
make_array (int *x[], int dim)
{
  for (int i = 0; i < DIM; i++)
    {
      x[i] = (int *) malloc (sizeof (int));
      *(x[i]) = i;
    }
}

int
check_array (int *x[], int dim)
{
  for (int i = 0; i < DIM; i++)
    if (*(x[i]) != -i)
      return 1;
  return 0;
}

int
main (void)
{
  int *x[DIM];
  make_array (x, DIM);

#pragma omp target data map (iterator(it = 0:DIM), tofrom: x[it][:1]) \
			map (to: x)
#pragma omp target
    {
      for (int i = 0; i < DIM; i++)
	x[i][0] = -x[i][0];
    }

  return check_array (x, DIM);
}
