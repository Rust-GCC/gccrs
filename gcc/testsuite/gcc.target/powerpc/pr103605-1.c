/* { dg-do compile } */
/* { dg-options "-O2 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */
/* { dg-final { scan-assembler-times {\mxsmaxdp\M} 3 } } */
/* { dg-final { scan-assembler-times {\mxsmindp\M} 3 } } */

/* Verify that GCC generates expected min/max hw insns instead of fmin/fmax
   calls.  */
#include "pr103605.h"
