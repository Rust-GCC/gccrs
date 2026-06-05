/* { dg-additional-options "-w" } */

double tetallnormal_A[1][4];
struct tetgenmesh {
  bool lu_decmp(double[][4], int *, double *, int);
  void tetallnormal();
};
int lu_decmp_n;
bool tetgenmesh::lu_decmp(double[][4], int *, double *d, int) {
  double biggest, pivotindex, k;
  *d = 1.0;
  for (; lu_decmp_n; k++)
    if (biggest == pivotindex != k)
      *d = -*d;
}
void tetgenmesh::tetallnormal() {
  double D;
  int indx[](lu_decmp(tetallnormal_A, indx, &D, 0));
}
