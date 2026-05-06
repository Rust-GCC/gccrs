/* { dg-additional-options "-O -fdump-analyzer -frounding-math" } */

void *p;

static inline void
bar(_Complex float f)
{
  __builtin_strcpy(p, (void *)&f); /* { dg-warning "uninit" } */
}

void
foo()
{
  bar(72057594037927934);
}
