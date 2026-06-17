/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -O3 -mrvv-vector-bits=zvl -fdump-rtl-avlprop-details" } */

long long a;
long b;
char c[169];
int d;

int main() {
  for (long g=0; g<13; ++g)
    for (long h=0; h<13; ++h)
      c[g*13 +h] = 8;

  for (unsigned g=0; g<13; g++)
    for (int h=0; h<13; h +=3)
      b += (c[g*13+h]) >> c[g*13 +9];

  a = (int)b;
  if (a != 0)
    __builtin_abort ();
}

/* { dg-final { scan-rtl-dump-not "Propagating AVL: \\(const_int 1 \\\[0x1\\\]\\)" "avlprop" } } */
