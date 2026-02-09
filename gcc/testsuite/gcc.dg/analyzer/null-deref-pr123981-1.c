/* { dg-do compile { target x86_64-*-* } } */
/* { dg-additional-options "-mfsgsbase" } */

#include <immintrin.h>
#define getgs() ((int __seg_gs *)0)

int main()
{
  static int gsval = 42;
  _writegsbase_u64((unsigned long long)&gsval);
  return *getgs(); /* { dg-bogus "-Wanalyzer-null-dereference" } */
}
