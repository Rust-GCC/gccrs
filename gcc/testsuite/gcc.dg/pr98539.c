/* { dg-do compile } */
/* { dg-options "-std=c99 -Wvla-parameter" } */

void foo(int n, double x[3][*]);
void foo(int n, double x[3][n]) { }

