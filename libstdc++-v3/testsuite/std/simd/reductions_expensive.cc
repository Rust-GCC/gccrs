// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }
// { dg-additional-options "-msse2" }
// { dg-timeout-factor 2 }
// { dg-require-effective-target run_expensive_tests }

#define EXPENSIVE_TESTS 1
#include "reductions.cc"
