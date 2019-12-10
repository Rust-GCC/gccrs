/* This is a copy of props-4.c but with variations in the attribute
   syntax.  */
/* { dg-do compile } */
/* { dg-options "-fgnu-tm -fdump-tree-tmedge -fdump-tree-tmmark" } */

int a, b;

void __attribute((transaction_may_cancel_outer,noinline)) cancel1()
{
  __transaction_cancel [[,,,outer,,]];
}

void
foo(void)
{
  __transaction_atomic [[__gnu__::__outer__]] {
    a = 2;
    __transaction_atomic {
      b = 2;
      cancel1();
    }
  }
}

/* { dg-final { scan-tree-dump-times " instrumentedCode" 1 "tmedge" } } */
/* { dg-final { scan-tree-dump-times "hasNoAbort" 0 "tmedge" } } */
