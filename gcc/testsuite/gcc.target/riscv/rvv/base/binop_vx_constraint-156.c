/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32d -O3 -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" } } */
#include "riscv_vector.h"

/*
** f1:
**	...
**	vmslt\.vx\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+
**  vmnot\.m\s+v[0-9]+,\s*v[0-9]+
**	vmslt\.vx\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+
**	vmandn\.mm\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+
**	...
**	ret
*/
void f1 (void * in, void * in2, void *out, int32_t x)
{
    vint32m1_t v = __riscv_vle32_v_i32m1 (in, 4);
    vint32m1_t v2 = __riscv_vle32_v_i32m1 (in2, 4);
    vbool32_t m3 = __riscv_vmsge_vx_i32m1_b32 (v, 17, 4);
    vbool32_t m4 = __riscv_vmsge_vx_i32m1_b32_mu (m3, m3, v2, 17, 4);
    __riscv_vsm_v_b32 (out, m4, 4);
}

/*
** f2:
**	...
**	vmslt\.vx\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+
**  vmnot\.m\s+v[0-9]+,\s*v[0-9]+
**	vmslt\.vx\tv[1-9][0-9]?,\s*v[0-9]+,\s*[a-x0-9]+,\s*v0.t
**	vmxor\.mm\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+
**	...
**	ret
*/
void f2 (void * in, void *out, int32_t x)
{
    vbool32_t mask = *(vbool32_t*)in;
    asm volatile ("":::"memory");
    vint32m1_t v = __riscv_vle32_v_i32m1 (in, 4);
    vint32m1_t v2 = __riscv_vle32_v_i32m1_m (mask, in, 4);
    vbool32_t m3 = __riscv_vmsge_vx_i32m1_b32 (v, 17, 4);
    vbool32_t m4 = __riscv_vmsge_vx_i32m1_b32_mu (mask, m3, v2, 17, 4);
    __riscv_vsm_v_b32 (out, m4, 4);
}

/*
** f3:
**	...
**	vmslt\.vx\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+
**  vmnot\.m\s+v[0-9]+,\s*v[0-9]+
**	vmslt\.vx\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+,\s*v0.t
**	vmxor\.mm\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+
**	...
**	ret
*/
void f3 (void * in, void *out, int32_t x)
{
    vbool32_t mask = *(vbool32_t*)in;
    asm volatile ("":::"memory");
    vint32m1_t v = __riscv_vle32_v_i32m1 (in, 4);
    vint32m1_t v2 = __riscv_vle32_v_i32m1_m (mask, in, 4);
    vbool32_t m3 = __riscv_vmsge_vx_i32m1_b32 (v, 17, 4);
    vbool32_t m4 = __riscv_vmsge_vx_i32m1_b32_m (m3, v2, 17, 4);
    __riscv_vsm_v_b32 (out, m4, 4);
}
