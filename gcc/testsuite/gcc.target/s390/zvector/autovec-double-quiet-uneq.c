/* { dg-do compile } */
/* { dg-options "-O3 -march=z13 -mzvector -mzarch" } */

#include "autovec.h"

AUTOVEC_DOUBLE (QUIET_UNEQ);

/* { dg-final { scan-assembler-times {\n\tvfchdb\t} 2 } } */
/* { dg-final { scan-assembler {\n\tvo\t} } } */
/* { dg-final { scan-assembler {\n\tvx\t} } } */
