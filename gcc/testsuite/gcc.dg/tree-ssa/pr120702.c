/* { dg-do compile } */
/* { dg-options "-Os -fdump-tree-optimized" } */

extern void *memcpy(void *, const void *, __SIZE_TYPE__);
extern int memcmp(const void *, const void *, __SIZE_TYPE__);

static const unsigned char orig[10]
  = { 'J', '2', 'O', 'Z', 'F', '5', '0', 'F', 'Y', 'L' };

static unsigned char test[10];

int
verify (void)
{
  return 0 == memcmp (test, orig, 10 * sizeof (orig[0]));
}

int
benchmark (void)
{
  memcpy (test, orig, 10 * sizeof (orig[0]));
  return 0;
}

/* { dg-final { scan-tree-dump-not " = \"J2OZF50FYL\";" "optimized" } } */
