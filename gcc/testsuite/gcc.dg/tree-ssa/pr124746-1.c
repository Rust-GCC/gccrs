/* { dg-do run } */
/* { dg-options "-O2 -fno-tree-fre -fdump-tree-phiprop-details" } */

/* PR tree-optimization/124746 */

struct a {
  int b;
} c, e = {1}, f;
int d, g, j, *k = &d, n;
static int i;
static struct a *h = &e;
int main() {
  for (; n < 1; n++) {
    int l;
    struct a *m = &c;
  L:
    if (k) {
      d = l = 0;
      for (; l < 1; l++) {
        j = i;
        if (j) {
          struct a **o = &m;
          if (g)
            goto L;
          *o = &f;
        }
      }
    } else
      *h = *m;
  }
  if (e.b != 1)
    __builtin_abort();
  return 0;
}

/* { dg-final { scan-tree-dump-not "Inserting PHI for result of load e =" "phiprop2" } }  */

