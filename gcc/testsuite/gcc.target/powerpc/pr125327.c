/* { dg-do compile } */
/* { dg-options "-O1 -mcpu=power8 -mno-vsx" } */

typedef double v2df __attribute__ ((vector_size (16)));

/* This is a test for preservation of target-specific options when optimization
   options are changed.  If the optimize() attribute is not applied correctly,
   then the values of any target-specific options already set by the target()
   attribute can be lost.  The vector/scalar (VSX) instruction enablement flag
   associated with foo() should be preserved when the optimization level is
   changed from 1 to 2, because neither -mvsx nor -mno-vsx is implied by -O2;
   however, an unrelated target-specific option, -munroll-only-small-loops, *is*
   implied by -O2, which requires the function's target option node to be
   rebuilt.  When that happens, the existing value of the VSX flag must be
   preserved.
 */

v2df __attribute__ ((target ("vsx"), optimize("O2")))
foo (double a, double b)
{
  return (v2df){a, b};
}

/* { dg-final { scan-assembler {\txxpermdi } } } */
