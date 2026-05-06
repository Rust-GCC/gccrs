/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-cddce1" } */

unsigned char  g1;
unsigned short g2;
unsigned int   g4;
unsigned long long g8;

/* Relaxed loads, result unused -- must be eliminated.  */
void test_relaxed_1 (void)
{
  unsigned char r;
  __atomic_load (&g1, &r, 0);
}

void test_relaxed_2 (void)
{
  unsigned short r;
  __atomic_load (&g2, &r, 0);
}

void test_relaxed_4 (void)
{
  unsigned int r;
  __atomic_load (&g4, &r, 0);
}

void test_relaxed_8 (void)
{
  unsigned long long r;
  __atomic_load (&g8, &r, 0);
}

/* Non-relaxed orders, result unused -- must be preserved.  */
void test_acquire (void)
{
  unsigned int r;
  __atomic_load (&g4, &r, 2);
}

void test_seqcst (void)
{
  unsigned int r;
  __atomic_load (&g4, &r, 5);
}

void test_consume (void)
{
  unsigned int r;
  __atomic_load (&g4, &r, 1);
}

/* Runtime order -- must be preserved (order unknown at compile time).  */
void test_runtime (int order)
{
  unsigned int r;
  __atomic_load (&g4, &r, order);
}

/* Relaxed load but result is used -- must be preserved.  */
unsigned int test_used (void)
{
  unsigned int r;
  __atomic_load (&g4, &r, 0);
  return r;
}

/* { dg-final { scan-tree-dump-times "__atomic_load_1" 0 "cddce1" } } */
/* { dg-final { scan-tree-dump-times "__atomic_load_2" 0 "cddce1" } } */
/* { dg-final { scan-tree-dump-times "__atomic_load_8" 0 "cddce1" } } */
/* { dg-final { scan-tree-dump-times "__atomic_load_4" 5 "cddce1" } } */
