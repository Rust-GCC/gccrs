/* This test needs runtime that provides stpcpy function.  */
/* { dg-do run { target *-*-linux* *-*-gnu* *-*-uclinux* } } */
/* { dg-options "-O2 -fdump-tree-strlen" } */

#define USE_GNU
#include "strlenopt-4.c"

/* { dg-final { scan-tree-dump-times "strlen \\(" 1 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 4 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 1 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 5 "strlen1" } } */
