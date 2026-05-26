/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-mapxf -march=x86-64 -O2 -mtune-ctrl=disable_setzucc" } */
/* { dg-final { scan-assembler-not "setzu" } } */

#include "apx-zu-3.c"
