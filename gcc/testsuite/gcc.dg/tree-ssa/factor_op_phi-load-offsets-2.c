/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt-details" } */

/* Support different offsets and different bases.  */

int test2(const int* data, const int *data1, bool b) {
    if (b)
        return data1[1];
    return data[2];
}
/* { dg-final { scan-tree-dump-times "was created due to different offsets" 1 "phiopt2" } } */
/* { dg-final { scan-tree-dump-times "changed to factor out load from" 1 "phiopt2" } } */
/* { dg-final { scan-tree-dump-times "was created for the pointers" 1 "phiopt2" } } */
