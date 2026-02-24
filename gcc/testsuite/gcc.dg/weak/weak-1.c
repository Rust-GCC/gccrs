/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-fno-common" } */
/* { dg-skip-if "" { *-*-mingw* } } */
/* NVPTX's definition of weak looks different to normal.  */
/* { dg-skip-if "" { nvptx-*-* } } */
/* { dg-skip-if PR119369 { amdgcn-*-* } } */

/* { dg-final { scan-weak "a" } } */
/* { dg-final { scan-weak "b" } } */
/* { dg-final { scan-weak "c" } } */
/* { dg-final { scan-weak "d" } } */
/* { dg-final { scan-weak "e" } } */
/* { dg-final { scan-weak "g" } } */
/* { dg-final { scan-not-weak "i" } } */
/* { dg-final { scan-weak "j" } } */

#pragma weak a
int a;

int b;
#pragma weak b

#pragma weak c
extern int c;
int c;

extern int d;
#pragma weak d
int d;

#pragma weak e
void e(void) { }

#if 0
/* This permutation is illegal.  */
void f(void) { }
#pragma weak f
#endif

#pragma weak g
int g = 1;

#if 0
/* This permutation is illegal.  */
int h = 1;
#pragma weak h
#endif

#pragma weak i
extern int i;

#pragma weak j
extern int j;
int use_j() { return j; }
