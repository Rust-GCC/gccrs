/* PR tree-optimization/48063 */
/* { dg-do compile } */
/* { dg-options "-std=gnu17 -fno-early-inlining" } */

extern void abort (void);
static void bar ();

void
foo ()
{
  bar (1);
}

static void
bar (double i)
{
  if (i)
    abort ();
}
