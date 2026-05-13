/* { dg-do compile } */
/* { dg-options "-Os" } */

int foo()
{
  return 200;
}

long fool()
{
  return 200;
}

int bar()
{
  return 512;
}

long barl()
{
  return 512;
}
/* { dg-final { scan-assembler-times "movl\[\\t \]*\\\$200, %eax" 2 } } */
/* { dg-final { scan-assembler-times "movl\[\\t \]*\\\$512, %eax" 2 } } */
