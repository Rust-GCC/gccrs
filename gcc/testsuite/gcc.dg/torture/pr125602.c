/* { dg-do compile } */
/* { dg-additional-options "-fsanitize=undefined" } */

void f7()
{
  signed char v10;
lbl_b1:
  v10 = -86 - 42 + v10;
  if (0 > v10) goto lbl_b1;
}
