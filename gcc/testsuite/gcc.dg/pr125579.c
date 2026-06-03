/* { dg-options "-w -O3 -fno-tree-ccp -fno-inline" } */

int printf(const char *, ...);
int a;
void c(int d) {
  unsigned e = 0;
  if (a)
    e = -(0 / (e * 0));
  while (a) {
    if (d)
      continue;
    printf("%d", e);
    e++;
  }
}
int main() { c(1); }
