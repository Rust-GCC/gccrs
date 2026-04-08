/* { dg-additional-options "-fno-tree-ch -fno-tree-dce -fno-tree-dominator-opts -fno-tree-vrp -fno-tree-dse" } */

int a, b[9], c, d;
static void e() {
  int f[9];
  unsigned char g;
h:
i:
  if (a)
    goto h;
  g = 0;
  for (; g < 9; g++) {
    b[g] = f[8] ? 0 : g;
    if (g)
      goto i;
    d ? c && d < 0 : c;
  }
}
int main() {
  while (c)
    e();
  return 0;
}
