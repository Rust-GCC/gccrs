/* { dg-do compile } */
/* { dg-additional-options "-fsanitize=undefined" } */

void f2(signed char a0)
{
  signed char v4;
  _Bool c7;
  signed char v8 = 91;
  v4 = v8 + v8;
  v8 = 15 - 50;
  a0 = v8 * v4;
  v8 = v4 - a0;
lbl_br10:
  c7 = a0 <= 0;
  if (c7)
    return;
  v4 = v8 - 24;
  a0 = a0 + v4;
  goto lbl_br10;
}
