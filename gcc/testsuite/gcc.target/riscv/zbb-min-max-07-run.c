/* { dg-do run { target { rv64 && riscv_zbb } } } */
/* { dg-options "-O2" } */

/* Runtime correctness checks for si3 min/max with edge values,
   verifying that the SUBREG_PROMOTED annotation does not break
   the result for boundary cases.  */

#define INT_MAX __INT_MAX__
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX (INT_MAX * 2U + 1U)

int __attribute__((noinline)) smin_si3(int a, int b) { return a < b ? a : b; }
int __attribute__((noinline)) smax_si3(int a, int b) { return a > b ? a : b; }
unsigned __attribute__((noinline)) umin_si3(unsigned a, unsigned b) { return a < b ? a : b; }
unsigned __attribute__((noinline)) umax_si3(unsigned a, unsigned b) { return a > b ? a : b; }

#define CHECK(expr) do { if (!(expr)) __builtin_abort (); } while (0)

int
main (void)
{
  /* smin edge cases */
  CHECK (smin_si3 (0, 0) == 0);
  CHECK (smin_si3 (1, -1) == -1);
  CHECK (smin_si3 (-1, 1) == -1);
  CHECK (smin_si3 (INT_MIN, INT_MAX) == INT_MIN);
  CHECK (smin_si3 (INT_MAX, INT_MIN) == INT_MIN);
  CHECK (smin_si3 (INT_MIN, 0) == INT_MIN);
  CHECK (smin_si3 (0, INT_MIN) == INT_MIN);
  CHECK (smin_si3 (INT_MAX, 0) == 0);
  CHECK (smin_si3 (INT_MIN, INT_MIN) == INT_MIN);
  CHECK (smin_si3 (INT_MAX, INT_MAX) == INT_MAX);

  /* smax edge cases */
  CHECK (smax_si3 (0, 0) == 0);
  CHECK (smax_si3 (1, -1) == 1);
  CHECK (smax_si3 (-1, 1) == 1);
  CHECK (smax_si3 (INT_MIN, INT_MAX) == INT_MAX);
  CHECK (smax_si3 (INT_MAX, INT_MIN) == INT_MAX);
  CHECK (smax_si3 (INT_MIN, 0) == 0);
  CHECK (smax_si3 (0, INT_MAX) == INT_MAX);
  CHECK (smax_si3 (INT_MIN, INT_MIN) == INT_MIN);
  CHECK (smax_si3 (INT_MAX, INT_MAX) == INT_MAX);

  /* umin edge cases -- bit-31 boundary is the key case */
  CHECK (umin_si3 (0U, 0U) == 0U);
  CHECK (umin_si3 (1U, 0U) == 0U);
  CHECK (umin_si3 (0U, 1U) == 0U);
  CHECK (umin_si3 (UINT_MAX, 0U) == 0U);
  CHECK (umin_si3 (0U, UINT_MAX) == 0U);
  CHECK (umin_si3 (UINT_MAX, UINT_MAX) == UINT_MAX);
  CHECK (umin_si3 (0x7FFFFFFFU, 0x80000000U) == 0x7FFFFFFFU);
  CHECK (umin_si3 (0x80000000U, 0x7FFFFFFFU) == 0x7FFFFFFFU);
  CHECK (umin_si3 (0x80000000U, 0x80000001U) == 0x80000000U);
  CHECK (umin_si3 (0xFFFFFFFEU, 0xFFFFFFFFU) == 0xFFFFFFFEU);

  /* umax edge cases */
  CHECK (umax_si3 (0U, 0U) == 0U);
  CHECK (umax_si3 (1U, 0U) == 1U);
  CHECK (umax_si3 (0U, 1U) == 1U);
  CHECK (umax_si3 (UINT_MAX, 0U) == UINT_MAX);
  CHECK (umax_si3 (0U, UINT_MAX) == UINT_MAX);
  CHECK (umax_si3 (UINT_MAX, UINT_MAX) == UINT_MAX);
  CHECK (umax_si3 (0x7FFFFFFFU, 0x80000000U) == 0x80000000U);
  CHECK (umax_si3 (0x80000000U, 0x7FFFFFFFU) == 0x80000000U);
  CHECK (umax_si3 (0x80000000U, 0x80000001U) == 0x80000001U);
  CHECK (umax_si3 (0xFFFFFFFEU, 0xFFFFFFFFU) == 0xFFFFFFFFU);

  return 0;
}
