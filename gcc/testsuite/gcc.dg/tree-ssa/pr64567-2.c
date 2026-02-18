/* { dg-do run } */
/* { dg-options "-O2" } */

void abort(void);

/* Macro adapted from builtin-object-size-common.h  */
#define FAIL() \
  do { \
    __builtin_printf ("Failure at line: %d\n", __LINE__);		      \
    abort();								      \
  } while (0)


__attribute__((noinline))
unsigned mask_zero_set (unsigned val, unsigned mask)
{
  if ((val & mask) == 0)
    val |= mask;

  return val;
}

__attribute__((noinline))
unsigned mask_notzero_clear (unsigned val, unsigned mask)
{
  if ((val & mask) != 0)
    val &= ~mask;

  return val;
}

int main (void) {
  if (mask_zero_set (0, 1) != 1)
    FAIL ();

  if (mask_zero_set (4, 3) != 7)
    FAIL ();

  if (mask_zero_set (1, 5) != 1)
    FAIL ();

  if (mask_notzero_clear (7, 1) != 6)
    FAIL ();

  if (mask_notzero_clear (7, 3) != 4)
    FAIL ();

  if (mask_notzero_clear (8, 6) != 8)
    FAIL ();

  return 0;
}
