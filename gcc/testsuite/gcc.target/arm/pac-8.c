/* Testing return address signing.  */
/* { dg-do run } */
/* { dg-require-effective-target mbranch_protection_ok } */
/* { dg-require-effective-target arm_pacbti_hw } */
/* { dg-options "-march=armv8.1-m.main+pacbti+fp -mbranch-protection=pac-ret+leaf -mthumb -mfloat-abi=hard --save-temps -O0" } */

#include <stdarg.h>
#include <stdlib.h>

int acc (int n, ...)
{
    int sum = 0;
    va_list ptr;

    va_start (ptr, n);

    for (int i = 0; i < n; i++)
        sum += va_arg (ptr, int);
    va_end (ptr);

    return sum;
}

int main()
{
  if (acc (3, 1, 2, 3) != 6)
    abort ();

  return 0;
}

/* { dg-final { scan-assembler-times "pac\tip, lr, sp" 2 } } */
/* { dg-final { scan-assembler-times "aut\tip, lr, sp" 2 } } */
/* { dg-final { scan-assembler-not "\tbti" } } */
