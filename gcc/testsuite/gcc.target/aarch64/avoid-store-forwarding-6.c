/* Check that the pass continues after a complex memory op (volatile load).  */
/* { dg-do compile } */
/* { dg-options "-O2 -favoid-store-forwarding -fdump-rtl-avoid_store_forwarding" } */

typedef union {
  char arr_8[8];
  long long_value;
} DataUnion;

long ssll_complex_mem (DataUnion *data, char x, volatile int *v)
{
  (void)*v;
  data->arr_8[4] = x;
  return data->long_value;
}

/* { dg-final { scan-rtl-dump-times "Store forwarding detected" 1 "avoid_store_forwarding" } } */
/* { dg-final { scan-rtl-dump-times "Store forwarding avoided" 1 "avoid_store_forwarding" } } */
