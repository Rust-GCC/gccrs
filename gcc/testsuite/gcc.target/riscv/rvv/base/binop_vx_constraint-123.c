/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32d -O3" } */
#include "riscv_vector.h"

void f (void * in, void *out, int32_t x)
{
    vint32mf2_t v = __riscv_vle32_v_i32mf2 (in, 4);
    vbool64_t mask = __riscv_vlm_v_b64 (in + 1, 4);
    vint32mf2_t v2 = __riscv_vle32_v_i32mf2 (in+444, 4);
    vbool64_t mask2 = __riscv_vmseq_vx_i32mf2_b64_m(mask,v,x,4);
    mask2 = __riscv_vmslt_vx_i32mf2_b64_mu(mask2,mask2,v2,x,4);
    __riscv_vsm_v_b64 (out, mask2, 4);
}

/* { dg-final { scan-assembler-not {vmv} } } */
