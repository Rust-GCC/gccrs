/* { dg-do compile } */
/* { dg-options "-O0 -mno-avx -msse2 -mtune=generic" } */

#include "builtin-memmove-18a.c"

/* { dg-final { scan-assembler "call	_?memmove" } } */
