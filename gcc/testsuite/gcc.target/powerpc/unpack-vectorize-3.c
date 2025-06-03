/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=power8 -mvsx -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fdump-tree-vect-details" } */
/* { dg-require-effective-target powerpc_vsx } */

/* Test if signed int unpack vectorization succeeds.  */

#include "unpack-vectorize-3.h"

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { scan-assembler-times {\mvupkhsw\M} 1 } } */
/* { dg-final { scan-assembler-times {\mvupklsw\M} 1 } } */
