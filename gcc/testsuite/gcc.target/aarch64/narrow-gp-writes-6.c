/* { dg-do compile } */
/* { dg-options "-O2 -mnarrow-gp-writes" } */
/* { dg-final { scan-assembler-times "\\tadd\\tw\[0-9\]+, w\[0-9\]+, 3" 1 } } */

unsigned long
test_phi3 (int a, int b, unsigned long val)
{
  unsigned long x;
  
  if (a) {
    if (b) {
      x = val & 0xFF;
    } else {
      x = val & 0xFF;
    }
  } else {
    x = val & 0xFF;
  }
  
  return x + 3;
}
