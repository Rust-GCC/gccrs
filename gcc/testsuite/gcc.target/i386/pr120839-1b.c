/* { dg-do compile } */
/* { dg-options "-O2 -mavx2" } */
/* { dg-final { scan-assembler-not "and\[lq\]?\[\\t \]*\\$-32,\[\\t \]*%\[re\]?sp" { xfail ia32 } } } */

#include "pr120839-1a.c"
