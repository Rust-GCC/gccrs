/* { dg-do compile } */
/* { dg-options "-O2 -march=z13" } */
/* { dg-final { scan-assembler-times "\tlarl\t" 1 } } */

/* Prior LRA we have

   12: {%f0:DF=[`*.LANCHOR0']+[const(`*.LANCHOR0'+0x8)];clobber %cc:CC;}

   and afterwards

   20: %r1:DI=`*.LANCHOR0'
   17: %f0:DF=[%r1:DI]
   19: %r1:DI=`*.LANCHOR0'
   12: {%f0:DF=%f0:DF+[%r1:DI+0x8];clobber %cc:CC;}

   where postreload removes the redundant insn 19.  */

double x, y;

double
test ()
{
  return x + y;
}
