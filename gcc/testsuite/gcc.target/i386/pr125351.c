/* { dg-do compile } */
/* { dg-options "-mptwrite -masm=intel" } */
void foo() { __builtin_ia32_ptwrite32(0); }
