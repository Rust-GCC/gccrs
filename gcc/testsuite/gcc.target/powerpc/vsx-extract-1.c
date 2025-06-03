/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-options "-O3 -mdejagnu-cpu=power7" } */
/* { dg-require-effective-target powerpc_vsx } */
/* { dg-final { scan-assembler     "lfd"    } } */
/* { dg-final { scan-assembler-not "lxvd2x" } } */

#include <altivec.h>

double get_value (vector double *p) { return vec_extract (*p, 0); }
