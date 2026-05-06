/* { dg-do run { target automatic_stack_alignment } } */
/* { dg-options "-O2" } */

#define ALIGNMENT	256

#include <stddef.h>

typedef struct {
  long double a;
  long double b;
} c __attribute__((aligned(ALIGNMENT)));

__attribute__ ((noipa))
void foo (c *p)
{
  if ((((ptrdiff_t) p) & (ALIGNMENT - 1)) != 0)
    __builtin_abort ();
}

__attribute__ ((noipa))
void
bar (c f)
{
  foo (&f);
}

c f;

int
main()
{
  bar (f);
  return 0;
}
