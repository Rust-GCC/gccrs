/* This tests whether REG_ALWAYS_RETURN notes are handled
   correctly in combine.  */
/* { dg-do compile { target fpic } } */
/* { dg-options "-O2 -fpic -fprofile-arcs" } */
/* { dg-require-profiling "-fprofile-generate" } */

int fork (void);

void test (void)
{
  fork ();
}
