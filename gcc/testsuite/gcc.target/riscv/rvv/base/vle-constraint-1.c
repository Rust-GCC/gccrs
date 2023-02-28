/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32d -O3" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "riscv_vector.h"

/*
** f1:
**	vsetivli\tzero,4,e32,m1,tu,ma
**	vle32\.v\tv[0-9]+,0\([a-x0-9]+\)
**	vle32\.v\tv[0-9]+,0\([a-x0-9]+\)
**	vse32\.v\tv[0-9]+,0\([a-x0-9]+\)
**	ret
*/
void f1 (float * in, float *out)
{
    vfloat32m1_t v = __riscv_vle32_v_f32m1 (in, 4);
    vfloat32m1_t v2 = __riscv_vle32_v_f32m1_tu (v, in, 4);
    __riscv_vse32_v_f32m1 (out, v2, 4);
}

/*
** f2:
**	vsetvli\t[a-x0-9]+,zero,e8,mf4,ta,ma
**	vlm.v\tv[0-9]+,0\([a-x0-9]+\)
**	vsetivli\tzero,4,e32,m1,ta,ma
**	vle32.v\tv[0-9]+,0\([a-x0-9]+\),v0.t
**	vse32.v\tv[0-9]+,0\([a-x0-9]+\)
**	ret
*/
void f2 (float * in, float *out)
{
    vbool32_t mask = *(vbool32_t*)in;
    asm volatile ("":::"memory");
    vfloat32m1_t v = __riscv_vle32_v_f32m1 (in, 4);
    vfloat32m1_t v2 = __riscv_vle32_v_f32m1_m (mask, in, 4);
    __riscv_vse32_v_f32m1 (out, v2, 4);
}

/*
** f3:
**	vsetvli\t[a-x0-9]+,zero,e8,mf4,ta,ma
**	vlm.v\tv[0-9]+,0\([a-x0-9]+\)
**	vsetivli\tzero,4,e32,m1,tu,mu
**	vle32\.v\tv[0-9]+,0\([a-x0-9]+\)
**	vle32.v\tv[0-9]+,0\([a-x0-9]+\),v0.t
**	vse32.v\tv[0-9]+,0\([a-x0-9]+\)
**	ret
*/
void f3 (float * in, float *out)
{
    vbool32_t mask = *(vbool32_t*)in;
    asm volatile ("":::"memory");
    vfloat32m1_t v = __riscv_vle32_v_f32m1 (in, 4);
    vfloat32m1_t v2 = __riscv_vle32_v_f32m1_tumu (mask, v, in, 4);
    __riscv_vse32_v_f32m1 (out, v2, 4);
}

/*
** f4:
**	vsetivli\tzero,4,e8,mf8,tu,ma
**	vle8\.v\tv[0-9]+,0\([a-x0-9]+\)
**	vle8\.v\tv[0-9]+,0\([a-x0-9]+\)
**	vse8\.v\tv[0-9]+,0\([a-x0-9]+\)
**	ret
*/
void f4 (int8_t * in, int8_t *out)
{
    vint8mf8_t v = __riscv_vle8_v_i8mf8 (in, 4);
    vint8mf8_t v2 = __riscv_vle8_v_i8mf8_tu (v, in, 4);
    __riscv_vse8_v_i8mf8 (out, v2, 4);
}

/*
** f5:
**	vsetvli\t[a-x0-9]+,zero,e8,mf8,ta,ma
**	vlm.v\tv[0-9]+,0\([a-x0-9]+\)
**	vsetivli\tzero,4,e8,mf8,ta,ma
**	vle8.v\tv[0-9]+,0\([a-x0-9]+\),v0.t
**	vse8.v\tv[0-9]+,0\([a-x0-9]+\)
**	ret
*/
void f5 (int8_t * in, int8_t *out)
{
    vbool64_t mask = *(vbool64_t*)in;
    asm volatile ("":::"memory");
    vint8mf8_t v = __riscv_vle8_v_i8mf8 (in, 4);
    vint8mf8_t v2 = __riscv_vle8_v_i8mf8_m (mask, in, 4);
    __riscv_vse8_v_i8mf8 (out, v2, 4);
}

/*
** f6:
**	vsetvli\t[a-x0-9]+,zero,e8,mf8,ta,ma
**	vlm.v\tv[0-9]+,0\([a-x0-9]+\)
**	vsetivli\tzero,4,e8,mf8,tu,mu
**	vle8\.v\tv[0-9]+,0\([a-x0-9]+\)
**	vle8.v\tv[0-9]+,0\([a-x0-9]+\),v0.t
**	vse8.v\tv[0-9]+,0\([a-x0-9]+\)
**	ret
*/
void f6 (int8_t * in, int8_t *out)
{
    vbool64_t mask = *(vbool64_t*)in;
    asm volatile ("":::"memory");
    vint8mf8_t v = __riscv_vle8_v_i8mf8 (in, 4);
    vint8mf8_t v2 = __riscv_vle8_v_i8mf8_tumu (mask, v, in, 4);
    __riscv_vse8_v_i8mf8 (out, v2, 4);
}
