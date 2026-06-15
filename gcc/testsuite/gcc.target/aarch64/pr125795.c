/* PR target/125795 */
/* { dg-do run } */
/* { dg-options "-O2 -funroll-loops" } */

/* This loop is vectorized with LD2 (deinterleave) and TBL (lane reversal),
   then unrolled.  The unrolled body builds a full-width (32 FPR) early-ra
   color group, which used to trigger a "1U << 32" undefined shift in
   early_ra::allocate_colors.  As a result the long-lived TBL permute index
   was wrongly allocated to the same FPR as an LD2 tuple destination and got
   clobbered, so the result was miscomputed.  */

extern void abort (void);

#define N 256

static int __attribute__ ((noipa))
foo (void)
{
  int k[N];
  int k2[N];
  int i;

  for (i = 0; i < N; i++)
    k2[i] = i;

  for (i = 0; i < 64; i++)
    {
      k[i] = k2[2 * i + 0];
      k[255 - i] = k2[2 * i + 1];
      k[64 + i] = k2[2 * i + 128];
      k[191 - i] = k2[2 * i + 129];
    }

  for (i = 0; i < N; i++)
    if (k[i] != ((i < 128) ? (2 * i) : (511 - 2 * i)))
      return 1;

  return 0;
}

int
main (void)
{
  if (foo ())
    abort ();
  return 0;
}
