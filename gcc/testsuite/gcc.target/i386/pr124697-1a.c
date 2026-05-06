/* { dg-do run  { target avx2_runtime } }  */
/* { dg-options "-O2 -mavx2" } */

typedef double v4df __attribute__((vector_size(32)));
typedef double v2df __attribute__((vector_size(16)));
typedef struct {
  v2df a[2];
} c __attribute__((aligned(32)));
v4df d;
c v;

__attribute__ ((noipa))
void
foo (float a1, float a2, float a3, float a4, float a5, float a6, c f)
{
  d = *(v4df *) &f;
}

__attribute__ ((noipa))
void
bar (void)
{
  foo (0, 0, 0, 0, 0, 0, v);
}

int
main ()
{
  bar ();
  return 0;
}
