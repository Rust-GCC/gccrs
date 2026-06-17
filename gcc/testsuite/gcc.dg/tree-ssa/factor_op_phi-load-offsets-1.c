/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt-details" } */

/* Support offsets that are different but the same base. */
int test2(const int* data, bool b) {
    if (b)
        return data[1];
    return data[0];
}
/* { dg-final { scan-tree-dump-times "was created for the index." 1 "phiopt2" } } */
/* { dg-final { scan-tree-dump-times "changed to factor out load from" 1 "phiopt2" } } */
