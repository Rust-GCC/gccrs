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

#pragma omp target enter data map (iterator(it = 0:DIM), to: x[it][:1]) \
			      map (to: x)
#pragma omp target
    {
      for (int i = 0; i < DIM; i++)
	x[i][0] = -x[i][0];
    }
#pragma omp target exit data map (iterator(it = 0:DIM), from: x[it][:1])
#pragma omp target exit data map (release: x)
  return check_array (x, DIM);
}
