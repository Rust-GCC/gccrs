/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -mcpu=neoverse-n2 --param=aarch64-autovec-preference=sve-only -msve-vector-bits=scalable" } */

#include <stdint.h>

/* Without an efficient implementation of vec_init for partial SVE types, a
   decision to vectorize a group in the basic block vectorizer can result in
   code that repeatedly stores a whole vector on the stack, overwrites one
   element, reloads the whole vector, stores it to another location,
   overwrites another element, etc. This is a fairly minimal reproducer.  */
void
vec_slp_pathological_stack (uint8_t *src)
{
  int lt = src[-33];
  int l0 = src[-1];
  int l1 = src[31];
  int t0 = src[-32];
  int t1 = src[-31];
  int t2 = src[-30];
  src[64] = (l1 + (2 * l0) + lt + 2) >> 2;
  src[65] = (lt + t0 + 1) >> 1;
  src[66] = (t0 + t1 + 1) >> 1;
  src[67] = (t1 + t2 + 1) >> 1;
}

/* { dg-final { scan-assembler-not {sp} } }
 */
