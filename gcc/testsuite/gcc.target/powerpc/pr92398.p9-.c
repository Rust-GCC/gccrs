/* { dg-do compile { target { lp64 && {! has_arch_pwr9} } } } */
/* { dg-options "-O2 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

/* { dg-final { scan-assembler-times {\mnot\M} 2 { xfail be } } } */
/* { dg-final { scan-assembler-times {\mstd\M} 2 { xfail { { {! has_arch_pwr9} && has_arch_pwr8 } && be } } } } */

/* Source code for the test in pr92398.h */
#include "pr92398.h"

