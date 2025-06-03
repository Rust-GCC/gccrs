/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc -mabi=lp64d -fstack-clash-protection --param stack-clash-protection-guard-size=16 -fomit-frame-pointer -momit-leaf-frame-pointer -fno-stack-protector" } */
/* { dg-require-effective-target supports_stack_clash_protection } */

void h (void) __attribute__ ((noreturn));

void
f (void)
{
  volatile int x[16384 + 1000];
  x[30]=0;
  h ();
}

/* { dg-final { scan-assembler-times {sd\tzero,1024\(sp\)} 1 } } */
/* { dg-final { scan-assembler-times {sw\tzero,120\(sp\)} 1 } } */

/* SIZE is more than 1 guard-size, but only one 64KB page is used, expect only 1
   probe.  Leaf function and omitting leaf pointers, tail call to noreturn which
   may only omit an epilogue and not a prologue.  Checking for LR saving.  */
