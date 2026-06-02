/* { dg-do compile } */
/* { dg-options "-march=c86-4g-m7 -O2 -fdump-tree-slp-details" } */
/* { dg-final { scan-tree-dump-times {(?n)Detected avx512 cross-lane permutation} 1 "slp2" } } */

void foo(double *a, double *__restrict b, int c, int n) {
    a[0] = b[100] * b[4];
    a[1] = b[100] * b[5];
    a[2] = b[100] * b[6];
    a[3] = b[100] * b[7];
    a[4] = b[100] * b[0];
    a[5] = b[100] * b[1];
    a[6] = b[100] * b[2];
    a[7] = b[100] * b[3];
}

void foo1(double *a, double *__restrict b, int c, int n) {
    a[0] = b[100] * b[0];
    a[1] = b[100] * b[1];
    a[2] = b[100] * b[3];
    a[3] = b[100] * b[2];
    a[4] = b[100] * b[4];
    a[5] = b[100] * b[5];
    a[6] = b[100] * b[7];
    a[7] = b[100] * b[6];
}
