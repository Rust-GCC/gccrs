/* { dg-do run  { target avx512f_runtime } }  */
/* { dg-options "-O2 -mavx512f" } */

typedef double v4df __attribute__((vector_size(32)));
typedef double v8df __attribute__((vector_size(64)));
typedef struct {
  v4df a[2];
} c __attribute__((aligned(64)));
v8df d;
c v;

__attribute__ ((noipa))
void
foo (float a1, float a2, float a3, float a4, float a5, float a6, c f)
{
  d = *(v8df *) &f;
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
