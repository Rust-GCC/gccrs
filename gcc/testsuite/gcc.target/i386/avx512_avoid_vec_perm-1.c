/* { dg-do compile } */
/* { dg-options "-O2 -march=c86-4g-m7 -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump "loop vectorized using 32 byte vectors" "vect" } } */

int a[512], b[512];

void foo(void) {
    int i;
    for (i = 0; i < 32; ++i) {
        b[i * 16 + 0] = a[i * 16 + 0];
        b[i * 16 + 1] = a[i * 16 + 0];
        b[i * 16 + 2] = a[i * 16 + 3];
        b[i * 16 + 3] = a[i * 16 + 5];
        b[i * 16 + 4] = a[i * 16 + 4];
        b[i * 16 + 5] = a[i * 16 + 6];
        b[i * 16 + 6] = a[i * 16 + 4];
        b[i * 16 + 7] = a[i * 16 + 9];
        b[i * 16 + 8] = a[i * 16 + 8];
        b[i * 16 + 9] = a[i * 16 + 8];
        b[i * 16 + 10] = a[i * 16 + 11];
        b[i * 16 + 11] = a[i * 16 + 13];
        b[i * 16 + 12] = a[i * 16 + 12];
        b[i * 16 + 13] = a[i * 16 + 14];
        b[i * 16 + 14] = a[i * 16 + 12];
        b[i * 16 + 15] = a[i * 16 + 14];
    }
}
