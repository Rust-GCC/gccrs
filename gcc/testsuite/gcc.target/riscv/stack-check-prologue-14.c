/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc -mabi=lp64d -fstack-clash-protection --param stack-clash-protection-guard-size=16 -fomit-frame-pointer -momit-leaf-frame-pointer -fno-stack-protector" } */
/* { dg-require-effective-target supports_stack_clash_protection } */

void h (void) __attribute__ ((noreturn));

void
f (void)
{
  volatile int x[16384 + 1000];
  if (x[0])
     h ();
  x[345] = 1;
  h ();
}

/* { dg-final { scan-assembler-times {sd\tzero,1024\(sp\)} 1 } } */
/* { dg-final { scan-assembler-times {sd\tra,8\(sp\)} 1 } } */

/* SIZE is more than 1 guard-size, two 64k pages used, expect only 1 explicit
   probe at 1024 and one implicit probe due to LR being saved.  Leaf function
   and omitting leaf pointers, tail call to noreturn which may only omit an
   epilogue and not a prologue and control flow in between.  Checking for
   LR saving.  */
