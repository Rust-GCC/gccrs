/* { dg-do run { target avx_runtime } } */
/* { dg-options "-fhardened -O3 -mavx" } */

struct a {
  unsigned long long b;
  unsigned long long c;
  int d;
  char e;
  int f;
} g;
int h = 1, i, j;
long long k, l, n;
int m[1]={};
short o;
static inline short p(short q, short r) { return q + r; }
int main() {
  for (; h < 3; h = p(h, 1))
    if (h) {
      k |= 4;
      struct a s = {8, 2, 2, 4, 15};
      for (int n = 0; n != 54; n++)
        if (m[l])
          if (i)
            o = 0;
      g = s;
      j &= 4073709551610LL;
    }
  return 0;
}
