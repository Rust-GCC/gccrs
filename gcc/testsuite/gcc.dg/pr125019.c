/* { dg-do compile } */
/* { dg-options "-O3 -fno-vect-cost-model -fno-tree-dce -fno-tree-pre" } */

int a, b, c, d;
void e(unsigned f) {
  for (c = 0; c < 2; c++) {
    b = d;
    d = f;
    for (a = 0; a < 2; a++)
      ;
  }
}
