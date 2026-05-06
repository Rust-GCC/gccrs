/* { dg-do compile } */
/* { dg-options "-O3 -funroll-loops -fdump-tree-lim2-details" } */
/* { dg-additional-options "--param max-completely-peeled-insns=200" { target { s390*-*-* } } } */
/* { dg-additional-options "--param max-completely-peeled-insns=300" { target { { arm*-*-* cris-*-* loongarch32-*-* m68k*-*-* } || { x86 || rv32 } } } } */
/* This param bump is only needed on x86 when sse2 is not enabled, but it
   doesn't hurt to use it on other x86 variants.  */

#define TYPE unsigned int

#include "pr83403.h"

/* { dg-final { scan-tree-dump-times "Executing store motion of" 10 "lim2" } } */
