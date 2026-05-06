/* { dg-do compile }  */
/* { dg-options "-O2 -mno-avx512f -mavx2" } */
/* { dg-final { scan-assembler-times "vmovapd.*, %ymm0" 1 } } */
/* { dg-final { scan-assembler-not "vmovupd.*, %ymm0" } } */
/* { dg-final { scan-assembler-not "and\[lq\]?\[\\t \]*\\$-64,\[\\t \]*%\[re\]?sp" } } */

typedef double v4df __attribute__((vector_size(32)));
typedef struct {
  v4df a[2];
} c __attribute__((aligned(64)));
v4df d;
c v;

void
foo (float a1, float a2, float a3, float a4, float a5, float a6, c f)
{
  d = *(v4df *) &f;
}
