/* { dg-additional-options "-fno-if-conversion -fharden-compares" } */

int a, b;

void
foo (void)
{
  b = a % 0 > 0;
}
