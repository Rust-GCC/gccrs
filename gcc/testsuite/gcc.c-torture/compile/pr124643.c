int a;
char *b;
void c(int a) {
  int d = (a & 2) != 0 && a & 1;
  for (;;)
    if (a)
      b[1 ^ d] = b[d];
}
