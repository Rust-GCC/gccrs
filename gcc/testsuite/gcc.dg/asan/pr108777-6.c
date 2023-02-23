/* PR sanitizer/108777 */
/* { dg-do compile } */
/* { dg-options "-O2 -fno-sanitize=all -fsanitize=kernel-address" } */
/* { dg-final { scan-assembler-not "__asan_memcpy" } } */
/* { dg-final { scan-assembler-not "__asan_memset" } } */
/* { dg-final { scan-assembler-not "__asan_memmove" } } */

void
foo (void *p, void *q, int s)
{
  __builtin_memcpy (p, q, s);
}

void
bar (void *p, void *q, int s)
{
  __builtin_memmove (p, q, s);
}

void
baz (void *p, int c, int s)
{
  __builtin_memset (p, c, s);
}
