/* { dg-do compile } */
/* { dg-require-effective-target vect_float } */

float *e;
void f (float *f, float *g, char *h, int n,
        int b, int c, int d)
{
  float a = 0;
  for (int i = 0; i < n; ++i) {
    int j = b + i, k = c + i * d;
    float l = g[j], m = h[i] ? g[k] : l;
    a += f[i] * m;
  }
  *e = a;
}

/* { dg-final { scan-tree-dump-not {failed: evolution of base is not affine} "vect" { target aarch64*-*-* } } } */
