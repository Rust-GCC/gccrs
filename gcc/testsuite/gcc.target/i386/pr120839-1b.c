/* { dg-do compile } */
/* { dg-options "-O2 -mavx2" } */
/* { dg-final { scan-assembler "and\[lq\]?\[\\t \]*\\$-32,\[\\t \]*%\[re\]?sp" } } */

#include "pr120839-1a.c"
