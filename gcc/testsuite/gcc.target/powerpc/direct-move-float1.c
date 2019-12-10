/* { dg-do compile { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-skip-if "" { powerpc*-*-*spe* } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-options "-mdejagnu-cpu=power8 -O2" } */
/* { dg-final { scan-assembler {\mmtvsrd\M}    } } */
/* { dg-final { scan-assembler {\mmfvsrwz\M}   } } */
/* { dg-final { scan-assembler {\mxscvdpspn\M} } } */
/* { dg-final { scan-assembler {\mxscvspdpn\M} } } */

/* Check code generation for direct move for float types.  */

#define TYPE float
#define IS_FLOAT 1
#define NO_ALTIVEC 1

#include "direct-move.h"
