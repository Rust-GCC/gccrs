/* Test multiple map clauses for the same variable with the 'always'
   map-type modifier.  */
/* { dg-do run { target offload_device_nonshared_as } } */

#define N 100

int
main (void)
{
  int a[N];

  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target data map(tofrom: a)
    {
      /* Device has a[i] = i.  Update the host copy to create divergence.  */
      for (int i = 0; i < N; i++)
	a[i] = i * 2;

      /* map(always, to) + map(tofrom): 'always' forces the updated host
	 values (i*2) onto the device despite the existing mapping.  */
      #pragma omp target map(always, to: a) map(tofrom: a)
	{
	  for (int i = 0; i < N; i++)
	    if (a[i] != i * 2)
	      __builtin_abort ();
          for (int i = 0; i < N; i++)
            a[i] = i * 3;
	}

      for (int i = 0; i < N; i++)
        if (a[i] != i * 2)
          __builtin_abort ();

      /* Reset host for the next test; device retains i*3.  */
      for (int i = 0; i < N; i++)
        a[i] = i;

      /* map(tofrom) + map(always, from): 'always' forces new device values
	 back to host.  */
      #pragma omp target map(tofrom: a) map(always, from: a)
	for (int i = 0; i < N; i++)
	  {
	    if (a[i] != i * 3)
	      __builtin_abort ();
	    a[i] = i * 4;
	  }
      for (int i = 0; i < N; i++)
	if (a[i] != i * 4)
	  __builtin_abort ();
    }

  return 0;
}
