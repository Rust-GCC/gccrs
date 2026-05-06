/* { dg-do compile } */
/* { dg-additional-options "-ffast-math" } */

struct S { double a, b, c, d, e, f; };
void g(struct S *s, double *p, double *q, long n)
{
  for (long i; i < n; ++i) {
    s->b = s->c * p[i];
    s->a = s->f * s->d + s->b;
    q[i] += s->f;
    if (s->e) s->a = s->e;
  }
}
