/* { dg-do compile }  */
/* { dg-options "-O2 -mno-avx512f -mavx2" } */
/* { dg-final { scan-assembler-times "vmovupd.*, %ymm0" 1 } } */
/* { dg-final { scan-assembler "and\[lq\]?\[\\t \]*\\$-32,\[\\t \]*%\[re\]?sp" } } */
/* { dg-final { scan-assembler-not "vmovapd.*, %ymm0" } } */

typedef double v4df __attribute__((vector_size(32)));
typedef double v2df __attribute__((vector_size(16)));
typedef struct {
  v2df a[2];
} c __attribute__((aligned(32)));
v4df d;
c v;

void
foo (float a1, float a2, float a3, float a4, float a5, float a6, c f)
{
  d = *(v4df *) &f;
}
