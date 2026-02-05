/* Check for a bug in the treatment of LOOP_VINFO_PARTIAL_LOAD_STORE_BIAS when
   using VEC_EXTRACT.  This variant uses .LEN_LOAD which might use QImode
   vectors during load, but SImode vectors for the extraction.  */
int __attribute__ ((noinline, noclone))
test_int (int *x, int n, int value)
{
  int last;
  for (int j = 0; j < n; ++j)
    {
      last = x[j];
      x[j] = last * value;
    }
  return last;
}

int
main ()
{
  int arr[] = {1,2,3,4,5,1};
  if (test_int (arr, sizeof (arr) / sizeof (arr[0]), 42) != 1)
    __builtin_abort();
  return 0;
}
