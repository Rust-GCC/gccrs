/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt-details" } */

/* Support of non-0 offsets. */

int test2(const int* data, const int *data1, bool b) {
    if (b)
        return data1[1];
    return data[1];
}
/* { dg-final { scan-tree-dump-not "was created due to different offsets" "phiopt2" } } */
/* { dg-final { scan-tree-dump-times "changed to factor out load from" 1 "phiopt2" } } */
/* { dg-final { scan-tree-dump-times "was created for the pointers" 1 "phiopt2" } } */
