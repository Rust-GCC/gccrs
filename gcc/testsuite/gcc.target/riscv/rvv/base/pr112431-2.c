/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -O3" } */

#include "riscv_vector.h"

size_t __attribute__ ((noinline))
sumation (size_t sum0, size_t sum1, size_t sum2, size_t sum3, size_t sum4,
	  size_t sum5, size_t sum6, size_t sum7)
{
  return sum0 + sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7;
}

size_t
foo (char const *buf, size_t len)
{
  size_t sum = 0;
  size_t vl = __riscv_vsetvlmax_e8m8 ();
  size_t step = vl * 4;
  const char *it = buf, *end = buf + len;
  for (; it + step <= end;)
    {
      vint8m2_t v0 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;
      vint8m2_t v1 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;
      vint8m2_t v2 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;
      vint8m2_t v3 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;
      vint8m2_t v4 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;
      vint8m2_t v5 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;
      vint8m2_t v6 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;
      vint8m2_t v7 = __riscv_vle8_v_i8m2 ((void *) it, vl);
      it += vl;

      asm volatile("nop" ::: "memory");
      vint16m4_t vw0 = __riscv_vwcvt_x_x_v_i16m4 (v0, vl);
      vint16m4_t vw1 = __riscv_vwcvt_x_x_v_i16m4 (v1, vl);
      vint16m4_t vw2 = __riscv_vwcvt_x_x_v_i16m4 (v2, vl);
      vint16m4_t vw3 = __riscv_vwcvt_x_x_v_i16m4 (v3, vl);
      vint16m4_t vw4 = __riscv_vwcvt_x_x_v_i16m4 (v4, vl);
      vint16m4_t vw5 = __riscv_vwcvt_x_x_v_i16m4 (v5, vl);
      vint16m4_t vw6 = __riscv_vwcvt_x_x_v_i16m4 (v6, vl);
      vint16m4_t vw7 = __riscv_vwcvt_x_x_v_i16m4 (v7, vl);

      asm volatile("nop" ::: "memory");
      size_t sum0 = __riscv_vmv_x_s_i16m4_i16 (vw0);
      size_t sum1 = __riscv_vmv_x_s_i16m4_i16 (vw1);
      size_t sum2 = __riscv_vmv_x_s_i16m4_i16 (vw2);
      size_t sum3 = __riscv_vmv_x_s_i16m4_i16 (vw3);
      size_t sum4 = __riscv_vmv_x_s_i16m4_i16 (vw4);
      size_t sum5 = __riscv_vmv_x_s_i16m4_i16 (vw5);
      size_t sum6 = __riscv_vmv_x_s_i16m4_i16 (vw6);
      size_t sum7 = __riscv_vmv_x_s_i16m4_i16 (vw7);

      sum += sumation (sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);
    }
  return sum;
}

/* { dg-final { scan-assembler-not {vmv1r} } } */
/* { dg-final { scan-assembler-not {vmv2r} } } */
/* { dg-final { scan-assembler-not {vmv4r} } } */
/* { dg-final { scan-assembler-not {vmv8r} } } */
/* { dg-final { scan-assembler-not {csrr} { xfail riscv*-*-* } } } */
