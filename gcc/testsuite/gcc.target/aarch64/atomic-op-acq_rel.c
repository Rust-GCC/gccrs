/* { dg-do compile } */
/* { dg-options "-march=armv8-a+nolse -O2 -mno-outline-atomics" } */

#include "atomic-op-acq_rel.x"

/* { dg-final { scan-assembler-times "ldaxr\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 6 } } */
/* { dg-final { scan-assembler-times "stlxr\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 6 } } */
