/* { dg-do run } */
/* { dg-options "-O0 -march=x86-64-v3" } */

#define preserve_none no_callee_saved_registers

#include "pr120870-1.c"
