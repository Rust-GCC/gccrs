/* { dg-do compile } */
/* { dg-options "-O2 -mbmi" } */
/* { dg-final { scan-assembler-not "blsmsk" } } */
/* { dg-final { scan-assembler-not "cmove" } } */

unsigned foo (unsigned a)
{
    if (a != 0)
     return __builtin_popcount (a ^ (a - 1)) - 1;
    else
     return 32;
}
