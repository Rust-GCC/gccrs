/* { dg-do compile } */
/* { dg-options "-Oz" } */

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
/* { dg-final { scan-assembler-times "xorl\[ \t\]*%eax, %eax" 4 } } */
/* { dg-final { scan-assembler-times "movb\[ \t\]*\\\$-56, %al" 2 } } */
/* { dg-final { scan-assembler-times "movb\[ \t\]*\\\$2, %ah" 2 } } */
