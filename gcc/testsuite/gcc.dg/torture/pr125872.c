/* { dg-additional-options "-fno-tree-dce" } */

void
foo(float f)
{
  for(;;) f *= (char)255;
}
