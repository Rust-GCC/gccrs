/* { dg-do run } */
/* { dg-options "-O3 -fno-tree-dse -fno-tree-dce -fdump-tree-cselim-details" } */
/* PR tree-optimization/125612 */

/* The conditional store of `h[2000]` should not become unconditional since
   it traps. */

int a, b, c = 2, d, e[4], f, i;
void g() {
  int h[5];
  e[0] = 2;
  while (1) {
    if (e[b]) {
      if (!c)
        h[2000] &= 1;
      return;
    }
  }
}
int main() {
  for (; i < 4; i++)
    g();
  return 0;
}

/* { dg-final { scan-tree-dump-not "Conditional store replacement happened" "cselim"} } */
