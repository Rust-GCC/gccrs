/* { dg-require-effective-target arm_v8_1m_mve_fp_ok } */
/* { dg-add-options arm_v8_1m_mve_fp } */
/* { dg-additional-options "-O2" } */

/* PR target/124870.  */

#include <arm_mve.h>

#ifdef __cplusplus
extern "C" {
#endif

void fn (int n, uint32_t *strides, float32_t *f)
{
    uint32x4_t vecScGathAddr = vld1q_u32(strides);
    float32x4_t vecA = vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
    float32x4_t vecB = vecA;
    int i;
    for (i = 0; i < n; ++i)
    {
        vecA = vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
        vecB = vaddq_f32 (vecB, vecA);
    }
    vstrwq_f32 (f, vecB);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-times "vadd.f32\tq\[0-9\]+, q\[0-9\]+, q\[0-9\]+" 1 } } */
