/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */
/* { dg-options "-march=rv64i_zfinx -mabi=lp64" } */
/* { dg-final { check-function-bodies "**" "" } } */

void foo(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float m0, float m1) {
/*
** foo:
**   ...
**   fadd.s\s*t0,\s*(a[0-5]|s[0-1]),\s*(a[0-5]|s[0-1])
**   ...
*/
    __asm__ volatile("fadd.s t0, %0, %0" : : "cf" (m0));
}
