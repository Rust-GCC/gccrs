/* Test multiple map clauses for the same variable with the 'present'
   map-type modifier.  */
/* { dg-do run } */

#define N 100

int
main (void)
{
  int a[N];

  for (int i = 0; i < N; i++)
    a[i] = i;

      /* { dg-output "libgomp: present clause: not present on the device \\(addr: 0x\[0-9a-f\]+, size: \[0-9\]+ \\(0x\[0-9a-f\]+\\), dev: \[0-9\]+\\\)" { target offload_device_nonshared_as } } */
    /* { dg-shouldfail "present error triggered" { offload_device_nonshared_as } } */
      #pragma omp target map(to: a) map(present, alloc: a) map(from: a)
        for (int i = 0; i < N; i++)
	  a[i] *= 2;

  return 0;
}
